NAME = philo

CFLAGS =  -Wall -Wextra -Werror

SRC = philo.c parse.c initial.c tools.c routine.c

OBJM = $(SRC:%.c=%.o)

all : $(NAME)

%.o : %.c philo.h
	cc $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJM)
	cc $(CFLAGS) $(OBJM) -o $(NAME)

clean : 
	rm -rf $(OBJM)

fclean : clean
	rm -rf $(NAME) 
re : fclean all