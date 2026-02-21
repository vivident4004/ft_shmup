NAME = ft_shmup
CC = c++
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lncurses -ltinfo

# SRCS = main.c entity.c game.c input.c physics.c render.c
SRCS = main.cpp game.cpp 
OBJS = $(SRCS:.c=.o)

all: $(NAME)

# Regola per il bonus
bonus: .bonus

.bonus: $(BONUSOBJS) $(OBJS)
	touch .bonus
	$(CC) $(CFLAGS) $(BONUSOBJS) $(LDFLAGS) -o $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per pulire i file generati
clean:
	rm -f $(OBJS) $(BONUSOBJS) .bonus

fclean: clean
	rm -f $(NAME)

re: fclean all