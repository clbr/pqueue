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
	unsigned long score;
	char value[10];

	struct list_head list;
	struct rb_node node;
};

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

		ttm_prio_add(&tree, new);
	}

	struct entry *tmp = ttm_prio_query_lowest(&tree);
	if (tmp) ttm_prio_remove(&tree, tmp);

	puts("Removed first. Cool, now checking 'em");

	struct rb_node *it = rb_first(&tree);
	for (; it; it = rb_next(it)) {
		struct entry *cur = container_of(it, struct entry, node);
		printf("Score %lu val %s\n", cur->score, cur->value);

		if (!list_empty(&cur->list)) {
			struct entry *mine;
			struct list_head *tgt = &cur->list;
			list_for_each_entry(mine, &cur->list, list) {
				printf("Score %lu val %s\n", mine->score, mine->value);
			}
		}
	}

	return 0;
}
