#include "test.h"

int main() {

	struct ttm_pqueue q;
	memset(&q, 0, sizeof(struct ttm_pqueue));

	struct ttm_pqueue_entry *entries = calloc(10, sizeof(struct ttm_pqueue_entry));

	unsigned i;
	for (i = 0; i < 10; i++) {
		ttm_prio_init_entry(&entries[i]);

		if (i % 2)
			ttm_prio_add(&q, &entries[i]);
	}

	for (i = 0; i < 10; i++) {
		if (ttm_prio_is_queued(&entries[i]) && !(i % 2))
			fail("queue\n");
	}

	return 0;
}
