CFLAGS = -Wall -Wextra -pthread
LIB_NAME =lib_codexion.a
HEADER = my_header.h
RM = rm -rf
SRCS = tools1.c heap.c edf.c fifo.c monitor.c coder_functions.c \
		dongle.c timer.c parse.c creating_functions.c free_functions.c tools2.c
OBJS = $(SRCS:.c=.o)


all: $(LIB_NAME) run

run:
	clear
	@gcc $(CFLAGS) main.c -L. -l_codexion -o codexion  
	./codexion 5 0 0 0 0 2 0 fifo

test:
	gcc test.c && ./a.out

$(LIB_NAME): $(OBJS)
	ar rcs $(LIB_NAME) $(OBJS)


%o: %c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(LIB_NAME) $(OBJS) codexion

re: fclean all