CFLAGS += -Wall -Wextra

NAME = kernmulti
SRC = $(wildcard *.c)

.PHONY: all clean test

all: $(NAME) test

$(NAME): $(SRC)
	$(CC) $(SRC) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(NAME)

test:
	$(MAKE) -C test
