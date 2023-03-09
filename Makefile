NAME = webserv
FLAGS = -Wall -Werror -Wextra std=c++98
SRC = $(wildcard *.cpp)
CC = c++
OBJ = ${SRC:.cpp=.o}

$(NAME): $(OBJ)
	$(CC) $(SRC) -o $@

all: $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re