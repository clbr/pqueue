#include "test.h"

int main() {

	struct ttm_pqueue q;
	memset(&q, 0, sizeof(struct ttm_pqueue));

	struct ttm_pqueue_entry *entries = calloc(10, sizeof(struct ttm_pqueue_entry));

	unsigned i;
	for (i = 0; i < 10; i++) {
		ttm_prio_init_entry(&entries[i]);
	}

	entries[0].score = 666;
	entries[1].score = 6666;
	entries[2].score = 66666;
	entries[3].score = 7;
	entries[4].score = 77;
	entries[5].score = 777;
	entries[6].score = 1;
	entries[7].score = 145;
	entries[8].score = 6;
	entries[9].score = 145;

	for (i = 0; i < 10; i++) {
		ttm_prio_add(&q, &entries[i]);
	}

	entries[1].score = 7;
	ttm_prio_add(&q, &entries[1]);

	entries[4].score = 7;
	ttm_prio_add(&q, &entries[4]);

	entries[5].score = 7;
	ttm_prio_add(&q, &entries[5]);

	entries[6].score = 145;
	ttm_prio_add(&q, &entries[6]);

	ttm_prio_remove(&q, &entries[3]);
	ttm_prio_remove(&q, &entries[1]);

	struct ttm_pqueue_entry *it = ttm_prio_query_lowest(&q);
	if (it->score != 6) fail("Wrong lowest score, got %u, expected %u\n",
				it->score, 6);
	for (i = 0; it; it = ttm_prio_query_next(it), i++) {
	}
	if (i != 9) fail("Wrong amount, got %u, expected %u\n", i, 9);

	return 0;
}
