NAME = webserv

FLAGS = -std=c++98 -Wall -Werror -Wextra -I headers

SRC_DIR = sources

SRC =	$(SRC_DIR)/Webserv.cpp\
		$(SRC_DIR)/InfoServer.cpp\
		$(SRC_DIR)/SetupCluster.cpp\
		$(SRC_DIR)/Server.cpp\
		$(SRC_DIR)/Request.cpp\
		$(SRC_DIR)/Response.cpp\
		$(SRC_DIR)/ctype.cpp\
		$(SRC_DIR)/cgi.cpp\
		$(SRC_DIR)/Parser.cpp\
		$(SRC_DIR)/utils.cpp\
		$(SRC_DIR)/ResponseUtil.cpp

CC = c++

OBJ = ${SRC:%.cpp=%.o}

%.o: %.cpp
	@$(CC) -c $< -o $@ -g $(FLAGS)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $^ -o $@
	@echo "\033[32mCOMPILATION SUCCESS 👌\033[0m"

clean:
	@rm -rf $(OBJ)
	@echo "\033[36mCLEAN 🗑️\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[36mFCLEAN 🗑️\033[0m"

re: fclean all

.PHONY: all clean fclean re