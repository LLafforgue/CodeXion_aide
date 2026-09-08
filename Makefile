NAME    = codexion

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pthread

SRCS    = codexion.c utils.c utils_free.c init.c threads.c prints.c stages.c strategies.c
OBJS    = $(SRCS:.c=.o)
HEADER  = codexion.h

ERROR_FIFO = 10 35 5 3 5 5 15 fifo
VALID_FIFO = 5 100 10 5 10 3 5 fifo
ERROR_EDF  = 5 100 50 10 100 3 5 edf
VALID_EDF  = 5 100 25 10 10 3 5 edf
BIG_TEST_FIFO = 100 10000 66 24 87 10 10 fifo
BIG_TEST_EDF = 100 10000 66 24 87 10 10 edf
ONE_CODER = 1 30 10 5 10 3 5 fifo

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
	@echo "=== 2. Cas valide edf (doit passer) ==="
	@./$(NAME) $(VALID_EDF)
	@echo "=== 3. error (ne doit pas passer) ==="
	@./$(NAME) $(ERROR_FIFO)
	@echo "=== 4. error (ne doit pas passer) ==="
	@./$(NAME) $(ERROR_EDF)
	@echo "=== 5. one_coder (ne doit pas passer) ==="
	@./$(NAME) $(ONE_CODER)

test_big: all
	@echo "=== 1. big fifo (doit passer) ==="
	@./$(NAME) $(BIG_TEST_FIFO)
	@echo "=== 2. big edf (doit passer) ==="
	@./$(NAME) $(BIG_TEST_EDF)


valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(VALID_FIFO)

.PHONY: all clean fclean re test test_big valgrind