#include "test.h"

int main() {

	struct ttm_pqueue q;
	memset(&q, 0, sizeof(struct ttm_pqueue));

	struct ttm_pqueue_entry *entries = calloc(10, sizeof(struct ttm_pqueue_entry));

	unsigned i;
	for (i = 0; i < 10; i++) {
		ttm_prio_init_entry(&entries[i]);
		entries[i].score = i / 3;
		ttm_prio_add(&q, &entries[i]);
	}

	for (i = 0; i < 10; i++) {
		entries[i].score = i / 2;
		ttm_prio_add(&q, &entries[i]);
	}

	struct ttm_pqueue_entry *it = ttm_prio_query_lowest(&q);
	for (i = 0; it; it = ttm_prio_query_next(it), i++) {
		if (it->score != i / 2) fail("Wrong score\n");
	}

	return 0;
}
