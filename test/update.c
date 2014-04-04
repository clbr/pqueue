#include "test.h"

int main() {

	struct ttm_pqueue q;
	memset(&q, 0, sizeof(struct ttm_pqueue));

	struct ttm_pqueue_entry *entries = calloc(10, sizeof(struct ttm_pqueue_entry));

	unsigned i;
	for (i = 0; i < 10; i++) {
		ttm_prio_init_entry(&entries[i]);
		entries[i].score = i;
		ttm_prio_add(&q, &entries[i]);
	}

	for (i = 0; i < 10; i++) {
		ttm_prio_add(&q, &entries[i]);
	}

	// There better be ten entries, scored correctly.

	struct ttm_pqueue_entry *it = ttm_prio_query_lowest(&q);
	if (it->score != 0) fail("Wrong score\n");

	i = 1;
	while (it = ttm_prio_query_next(it)) {
		if (it->score != i) fail("Wrong score\n");
		i++;
	}

	return 0;
}
