NAME    = codexion

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pthread

SRCS    = codexion.c utils.c init.c threads.c
OBJS    = $(SRCS:.c=.o)
HEADER  = codexion.h

VALID_FIFO = 6 200 100 100 100 3 50 fifo
VALID_EDF  = 6 200 100 100 100 3 50 edf

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