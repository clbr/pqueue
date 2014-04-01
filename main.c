#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#include "list.h"
#include "ttm_priority.h"

#define container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct entry {
	char value[10];

	struct ttm_pqueue_entry p;
};

int main() {

	printf("Add entries like [score value]\n");

	struct ttm_pqueue queue;
	queue.tree = RB_ROOT;

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
		new->p.score = score;
		strncpy(new->value, ptr, 10);

		ttm_prio_add(&queue, &new->p);
	}

	struct ttm_pqueue_entry *tmp = ttm_prio_query_lowest(&queue);
	if (tmp) ttm_prio_remove(&queue, tmp);

	puts("Removed first. Cool, now checking 'em");

	struct rb_node *it = rb_first(&queue.tree);
	for (; it; it = rb_next(it)) {
		struct ttm_pqueue_entry *cur = container_of(it, struct ttm_pqueue_entry, node);
		struct entry *foo = container_of(cur, struct entry, p);
		printf("Score %llu val %s\n", cur->score, foo->value);

		if (!list_empty(&cur->list)) {
			struct ttm_pqueue_entry *mine;
			list_for_each_entry(mine, &cur->list, list) {
				foo = container_of(mine, struct entry, p);
				printf("underScore %llu val %s\n", mine->score, foo->value);
			}
		}
	}

	return 0;
}
