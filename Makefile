NAME    = codexion

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pthread

SRCS    = codexion.c utils.c utils_free.c init.c threads.c prints.c stages.c strategies.c
OBJS    = $(SRCS:.c=.o)
HEADER  = codexion.h

VALID_FIFO = 1 30 10 5 10 3 5 fifo
VALID_EDF  = 5 100 50 100 100 3 5 edf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -g -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	@echo "=== 1. Cas valide fifo (doit passer) ==="
	@./$(NAME) $(VALID_FIFO)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(VALID_FIFO)

.PHONY: all clean fclean re test valgrind