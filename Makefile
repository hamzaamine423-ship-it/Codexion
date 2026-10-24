CFLAGS = -Wall -Wextra -Werror -pthread 
LIB_NAME = lib_codexion.a
HEADER = codexion_header.h
RM = rm -rf
SRCS = tools1.c heap.c edf.c fifo.c monitor.c coder_functions.c \
		dongle.c timer.c parse.c creating_functions.c free_functions.c tools2.c\
		 edf2.c fifo2.c queue.c \

OBJS = $(SRCS:.c=.o)

all: $(LIB_NAME) run

$(LIB_NAME): $(OBJS)
	ar rcs $(LIB_NAME) $(OBJS)

%.o: %.c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@

run: 
	gcc $(CFLAGS) codexion.c -L. -l_codexion -o codexion
	./codexion 2 3000 200 200 200 1 400 edf

clean: 
	$(RM) $(OBJS)

fclean:
	$(RM) $(LIB_NAME) $(OBJS) codexion

re : fclean all

.PHONY: run clean fclean all re