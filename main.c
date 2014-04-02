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

	tmp = ttm_prio_query_lowest(&queue);
	for (; tmp; tmp = ttm_prio_query_next(tmp)) {
		struct entry *foo = container_of(tmp, struct entry, p);
		printf("Score %llu val %s\n", tmp->score, foo->value);
	}

	return 0;
}
