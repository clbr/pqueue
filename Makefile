CFLAGS += -Wall -Wextra

NAME = kernmulti
SRC = $(wildcard *.c)

.PHONY: all clean

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(SRC) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o $(NAME)
