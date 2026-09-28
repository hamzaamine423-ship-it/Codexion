CFLAGS= -Wall -Wextra -Werror

LIB_NAME=lib_codexion.a
HEADER= my_header.h
RM= rm -rf
SRCS= tools1.c
OBJS= $(SRCS:.c=.o)


all: $(LIB_NAME)
	gcc parse.c -L. -l_codexion -o codexion && ./codexion 5  600 200 200 200 1 200 edf   

run:
	

$(LIB_NAME): $(OBJS)
	@ar rcs $(LIB_NAME) $(OBJS)


%o: %c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(LIB_NAME) $(OBJS)

re: fclean all