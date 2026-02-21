NAME = ft_shmup
SRC = main.cpp game.cpp player.cpp 

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -lncurses -ltinfo

#Colori ANSI
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
RESET = \033[0m

all: $(NAME)

$(NAME): $(SRC)
	@$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)
	@echo "$(GREEN)✔ Eseguibile $(YELLOW)$(NAME)$(RESET) $(GREEN)generato$(RESET)"

clean:
	@rm -f $(NAME)
	@echo "$(BLUE)➜ Eseguibile $(YELLOW)$(NAME)$(RESET) $(BLUE)rimosso$(RESET)"

fclean: clean

re: fclean all

.PHONY: all clean fclean re