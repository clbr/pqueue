#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#include "list.h"

#define container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct entry {
	unsigned long score;
	char value[10];

	struct list_head list;
	struct rb_node node;
};

void insert(struct rb_root *root, struct entry *data) {

	struct rb_node **new = &root->rb_node, *parent = NULL;

	while (*new) {
		struct entry *this = container_of(*new, struct entry, node);
		parent = *new;

		if (data->score < this->score)
			new = &((*new)->rb_left);
		else if (data->score > this->score)
			new = &((*new)->rb_left);
		else
			return;
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);
}

struct entry *search(struct rb_root *root, unsigned long score) {

	struct rb_node **new = &root->rb_node;

	while (*new) {
		struct entry *this = container_of(*new, struct entry, node);

		if (score < this->score)
			new = &((*new)->rb_left);
		else if (score > this->score)
			new = &((*new)->rb_left);
		else
			return this;
	}

	return NULL;
}

int main() {

	printf("Add entries like [score value]\n");

	struct rb_root tree = RB_ROOT;

	char buf[160];
	while (fgets(buf, 160, stdin)) {
		if (strlen(buf) < 2) break;

		unsigned long score = atol(buf);
		char *ptr = buf;
		while (*ptr != ' ' && *ptr);
		ptr++;

		struct entry *new = calloc(1, sizeof(struct entry));
		new->score = score;
		strncpy(new->value, ptr, 10);
	}

	while (1) {
	
	}

	return 0;
}
