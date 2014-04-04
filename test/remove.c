#include "test.h"

static void count(struct ttm_pqueue_entry *it, const unsigned expected) {

	unsigned i;
	for (i = 0; it; it = ttm_prio_query_next(it)) i++;
	if (i != expected) fail("Wrong amount, got %u, expected %u\n", i, expected);
}

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

	count(ttm_prio_query_lowest(&q), 10);

	entries[1].score = 7;
	ttm_prio_add(&q, &entries[1]);

	count(ttm_prio_query_lowest(&q), 10);

	entries[4].score = 7;
	ttm_prio_add(&q, &entries[4]);

	count(ttm_prio_query_lowest(&q), 10);

	entries[5].score = 7;
	ttm_prio_add(&q, &entries[5]);

	count(ttm_prio_query_lowest(&q), 10);

	entries[6].score = 145;
	ttm_prio_add(&q, &entries[6]);

	count(ttm_prio_query_lowest(&q), 10);

	ttm_prio_remove(&q, &entries[3]);
	ttm_prio_remove(&q, &entries[1]);

	if (ttm_prio_is_queued(&entries[3])) fail("A removed was queued?\n");
	if (ttm_prio_is_queued(&entries[1])) fail("A removed was queued?\n");

	for (i = 0; i < 10; i++) {
		if (i == 1 || i == 3)
			continue;
		if (!ttm_prio_is_queued(&entries[i])) fail("An added was not queued?\n");
	}

	struct ttm_pqueue_entry *it = ttm_prio_query_lowest(&q);
	if (it->score != 6) fail("Wrong lowest score, got %u, expected %u\n",
				it->score, 6);
	count(it, 8);

	return 0;
}
