CFLAGS= -Wall -Wextra 

LIB_NAME=lib_codexion.a
HEADER= my_header.h
RM= rm -rf
SRCS= tools1.c parse.c creating_functions.c free_functions.c
OBJS= $(SRCS:.c=.o)


all: $(LIB_NAME) run
	 
run:
	gcc main.c -L. -l_codexion -o codexion  
	./codexion 1 600 200 200 200 1 200 edf

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