NAME = training_camp_philo

CFLAGS = -Wall -Werror -Wextra -I point_hache.h

SRC =	main.c utils.c

OBJ = $(SRC:.c=.o) 

all : $(NAME)

$(NAME) : $(OBJ)
			gcc -pthread -o $(NAME) $(OBJ) $(CFLAGS)

clean :
			rm -rf $(OBJ)

fclean : clean
			rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re