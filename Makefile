NAME = philo

CFLAGS =  -Wall -Wextra -Werror #-fsanitize=thread -g
SRC = philo.c

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