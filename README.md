*This project has been created as part of the 42 curriculum by hamine.*

# Codexion

> Master the race for resources before the deadline masters you.

## Description

Codexion is a concurrency simulation written in C with POSIX threads. It is a variant of the classic **Dining Philosophers problem**.

A group of coders sits around a circular table. Between each pair of neighbours there is one USB dongle (as many dongles as coders). To compile, a coder needs **two dongles at the same time**: the one on their left and the one on their right. Each coder repeats the same cycle:

```
compile  ->  debug  ->  refactor  ->  compile  -> ...
```

Three rules make the problem interesting:

- **Burnout.** A coder who does not *start* a compile within `time_to_burnout` ms of the start of their previous compile (or of the simulation) burns out, and the simulation stops.
- **Cooldown.** After a dongle is released it cannot be taken again for `dongle_cooldown` ms.
- **Scheduling.** When several coders compete for the same dongles, access is granted according to a policy: `fifo` (arrival order) or `edf` (Earliest Deadline First, where the deadline is `last_compile_start + time_to_burnout`).

The goal is to coordinate the threads so that nobody duplicates a dongle, nothing deadlocks, nobody starves, the cooldown is respected, and a burnout is reported within 10 ms of when it actually happens.

### Overview of the implementation

- Each coder is a thread (`pthread_create`).
- A separate **monitor thread** detects burnout and the end of the simulation.
- Each dongle has its own mutex protecting its state (taken or free, time of last release).
- `fifo` uses a **circular queue**; `edf` uses a **binary min-heap** (no standard library priority queue). Both only decide the order in which coders are allowed to go for the dongles.
- A shared stop flag, protected by a mutex, tells every thread to finish.

## Instructions

### Requirements

Linux, `cc`, `make`, POSIX threads.

### Compilation

```bash
make          # builds ./codexion with: cc -Wall -Wextra -Werror -pthread
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # fclean + all
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
|---|---|
| `number_of_coders` | Number of coders (and of dongles) |
| `time_to_burnout` | ms a coder may go without starting a compile |
| `time_to_compile` | ms spent compiling (holding two dongles) |
| `time_to_debug` | ms spent debugging |
| `time_to_refactor` | ms spent refactoring |
| `number_of_compiles_required` | The simulation stops when every coder has compiled at least this many times |
| `dongle_cooldown` | ms a released dongle stays unavailable |
| `scheduler` | `fifo` or `edf` |

All eight arguments are mandatory. Invalid input (wrong number of arguments, negative or non-integer values, a scheduler other than `fifo` or `edf`) is rejected before any thread is created.

### Examples

```bash
./codexion 5 800 200 200 200 4 10 fifo
./codexion 5 1200 200 100 100 6 100 edf
./codexion 1 800 200 200 200 3 0 fifo     # a single coder always burns out
```

### Output format

Every state change is printed as `timestamp_in_ms coder_id message`, with the timestamp relative to the start of the simulation:

```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
400 1 is refactoring
1204 3 burned out
```

The simulation ends when a coder burns out, or when every coder has compiled at least `number_of_compiles_required` times.

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

A deadlock needs four conditions at once. Preventing it means breaking at least one.

| Condition | Status in this project |
|---|---|
| Mutual exclusion | Kept. A dongle is exclusive by definition. |
| Hold and wait | Kept in a limited form: a coder locks the first dongle's mutex, then the second. |
| No preemption | Kept. A dongle is only released by the coder that holds it. |
| **Circular wait** | **Broken.** See below. |

With everyone locking "left, then right", all coders could each take one dongle and wait forever for their neighbour's, forming a circle. To break it, the lock order is **asymmetric**: coder 1 locks its left dongle then its right dongle, while every other coder locks its right dongle then its left one. In a ring, this means at least one coder acquires in the opposite direction from the others, so a full circle of "holds one, waits for the next" can never form.

Other deadlock sources avoided:

- Every thread leaves its loop when the stop flag is set, and every lock taken on the way is released, so `pthread_join` never waits for a thread blocked on a mutex.
- With a single coder there is only one dongle, so a coder never tries to lock the same mutex twice.

### Starvation prevention

- With `fifo`, coders enter a circular queue when they want to compile, and only the coder at the front goes for the dongles.
- With `edf`, coders enter a min-heap ordered by `last_compile` (the start of their last compile). Since `time_to_burnout` is the same for everybody, the smallest `last_compile` is also the earliest deadline `last_compile + time_to_burnout`, so the coder closest to burnout is released first.
- Equal deadlines in `edf` are resolved by the position of the coders in the heap (the sift comparisons are strict), not by an explicit secondary key.

### Cooldown handling

Each dongle stores the time it was last released. A coder takes its two dongles only when **both** are free and the current time is past `released + dongle_cooldown` for both. A dongle that has never been used has no cooldown. Because nobody signals when a cooldown ends, the coder re-checks the condition at short intervals until it holds.

### Precise burnout detection

A dedicated monitor thread runs independently of the coders. Every cycle (about 1 ms) it checks each coder that has not yet finished its required compiles and compares the current time with `last_compile + time_to_burnout`. `last_compile` is written at the **start** of a compile, as the subject defines the deadline. The burnout message is therefore printed within a few ms of the real burnout, well inside the 10 ms limit. Reading `last_compile` is protected by a per-coder mutex, so the monitor never sees a half-written value.

### Log serialization

All output goes through one function protected by a mutex, so two messages never interleave on one line. Once the stop flag is set, no further state line is printed after the `burned out` message.

### Other cases

- **One coder:** one dongle, so compiling is impossible. The coder burns out at `time_to_burnout`, and the program exits cleanly.
- **Early finish:** when every coder has compiled enough times, the monitor sets the stop flag and every thread exits its loop on its next check.
- **Memory:** everything is allocated once, joined and freed at exit (checked with valgrind).

## Thread synchronization mechanisms

### Primitives used

| Primitive | Protects | Used by |
|---|---|---|
| `d_mutex` (one per dongle) | The state of that dongle: taken or free, release time | The two coders sharing it |
| `c_mutex` (one per coder) | `last_compile` and `compile_count` | The coder and the monitor |
| `global_mutex` | The `simulation_stop` flag | Every coder and the monitor |
| `q_mutex` / `h_mutex` | The FIFO queue / the EDF heap | Every coder |
| output mutex | The standard output | Every thread that prints |

`pthread_cond_t` variables are initialized in the queue and heap structures, but waiting is implemented as **polling**: a waiting thread sleeps a few microseconds (`usleep`), then locks the mutex and re-checks its condition. The cost of this choice is CPU time. The benefit is that every waiting loop re-reads the stop flag on each iteration, so no thread can sleep through the end of the simulation.

### How race conditions are prevented

**1. Duplicated dongles.** Two neighbours could both see a dongle as free and both take it. The check "is it free?" and the update "now it is taken" happen while the dongle's mutex is held, so they form one indivisible step. The second coder then sees `taken` and does not take it.

**2. Queue and heap corruption.** Pushes and pops modify the size and the array. Each is done with the queue or heap mutex held, so two threads can never modify them at once.

**3. Monitor reading while a coder writes.** The coder writes `last_compile` at the start of each compile and the monitor reads it every millisecond. Both access it under the coder's `c_mutex`, so the monitor never reads a partially updated value.

**4. Stopping the simulation.** The monitor sets `simulation_stop` under `global_mutex`. Every other thread reads the flag through a function that takes the same mutex, so the change is seen safely.

### Communication between the coders and the monitor

- **Coders to monitor:** coders publish `last_compile` and `compile_count` under their own `c_mutex`. The monitor only reads shared state; it does not interact with the coders directly.
- **Monitor to coders:** the monitor sets the stop flag. Every waiting loop and every phase of the coder cycle checks it, then releases any lock it holds and returns, so the main thread can join all threads and free the memory.

## Known limitations

- Between leaving the queue (or heap) and locking the dongle mutexes there is a small window in which the order chosen by the scheduler is not enforced by the mutexes themselves.
- A coder polls while holding both of its dongle mutexes, so a neighbour that wants one of them waits inside `pthread_mutex_lock`. Polling uses CPU time, and it grows with the number of coders.
- `edf` ties are not broken by an explicit secondary key.

## Testing

Compile counts per coder (they should all be equal):

```bash
./codexion 5 1200 200 100 100 6 100 edf | grep "is compiling" | awk '{print $2}' | sort -n | uniq -c
```

Checks worth running on every scenario:

- No `burned out` line when the parameters are feasible (`time_to_burnout` above both `compile + debug + refactor` and `(n / floor(n/2)) * (compile + cooldown)`).
- Two neighbouring coders never have overlapping compile windows, and a dongle is not reused before its cooldown ends.
- A burnout line appears within 10 ms of the real burnout and is the last line printed.
- The program exits by itself, with no hang.
- Repeat each test many times, with both schedulers, under `valgrind --leak-check=full` and `valgrind --tool=helgrind` (or `-fsanitize=thread`).

## Resources

**Classic references**

- [Understanding Heaps and Priority Queues — Explains heap data structures and how priority queues organize elements by priority.](https://www.youtube.com/watch?v=HqPJF2L5h9U&t=1721s)

- [Understanding Thread Logic and Thread Functions — Introduces multithreading concepts and thread functions for understanding concurrent program execution.](https://www.youtube.com/watch?v=d9s_d28yJq0&t=3s)



**How AI was used**

An AI assistant (Claude) was used as a learning partner and reviewer, not to write the project code.

- *Concepts:* explanations of threads and stacks, mutexes, condition variables, race conditions, deadlock and Coffman's conditions, starvation, EDF scheduling, and the time functions (`gettimeofday`, `clock_gettime`).
- *Practice:* a few small standalone exercises on `pthread_cond_wait` / `pthread_cond_signal`. They are not part of this project.
- *Review:* it pointed out problems in my code, which I then fixed myself: locking a mutex that was already held in the heap wait loop, a lock-order cycle between dongles, unprotected reads of shared variables, a heap that compared two different keys, and a monitor that was blocked by a held lock.
- *Testing:* suggestions for test parameters and a feasibility formula for the parameters.
- *Documentation:* a first draft of this README, which I reviewed against my code and edited.

The scheduler (queue and heap), the dongle logic, the coder cycle and the monitor were written by me, and I can explain each of them.