SRC = $(wildcard *.c)
TARGETS = $(SRC:.c=)

CFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)
	@./run-tests.sh $(TARGETS)

$(TARGETS): ../ttm_priority.c ../rbtree.c

clean:
	rm -f *.o $(TARGETS)
