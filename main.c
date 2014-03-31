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

void insert_rb(struct rb_root *root, struct entry *data) {

	struct rb_node **new = &root->rb_node, *parent = NULL;

	while (*new) {
		struct entry *this = container_of(*new, struct entry, node);
		parent = *new;

		if (data->score < this->score)
			new = &((*new)->rb_left);
		else if (data->score > this->score)
			new = &((*new)->rb_right);
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
			new = &((*new)->rb_right);
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

		ptr = strchr(ptr, '\n');
		if (ptr) *ptr = '\0';

		ptr = buf;

		while (*ptr != ' ' && *ptr) ptr++;
		ptr++;

		struct entry *new = calloc(1, sizeof(struct entry));
		new->score = score;
		strncpy(new->value, ptr, 10);
		INIT_LIST_HEAD(&new->list);

		struct entry *test = search(&tree, score);
		if (!test) {
			insert_rb(&tree, new);
		} else {
			list_add_tail(&new->list, &test->list);
		}
	}

	puts("Cool, now checking 'em");

	struct rb_node *it = rb_first(&tree);
	for (; it; it = rb_next(it)) {
		struct entry *cur = container_of(it, struct entry, node);
		printf("Score %lu val %s\n", cur->score, cur->value);

		if (!list_empty(&cur->list)) {
			struct list_head *pos;
			list_for_each(pos, &cur->list) {
				cur = container_of(pos, struct entry, list);
				printf("Score %lu val %s\n", cur->score, cur->value);
			}
		}
	}

	return 0;
}
