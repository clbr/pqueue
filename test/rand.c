#include "test.h"

int main() {

	const int size = 1000;

	struct ttm_pqueue q;
	memset(&q, 0, sizeof(struct ttm_pqueue));
	srand(42);

	struct ttm_pqueue_entry *entries = calloc(size, sizeof(struct ttm_pqueue_entry));
	unsigned *removed = calloc(size, sizeof(unsigned));

	unsigned i;
	for (i = 0; i < size; i++) {
		ttm_prio_init_entry(&entries[i]);
		entries[i].score = rand() % (size / 5);
		ttm_prio_add(&q, &entries[i]);
	}

	unsigned total = size;
	for (i = 0; i < size / 2; i++) {
		unsigned which = rand() % size;
		if (!removed[which]) {
			ttm_prio_remove(&q, &entries[which]);
			removed[which] = 1;
			total--;
		}
	}

	for (i = 0; i < size / 2; i++) {
		unsigned which = rand() % size;
		if (!removed[which]) {
			ttm_prio_add(&q, &entries[which]);
		}
	}

	for (i = 0; i < size / 2; i++) {
		unsigned which = rand() % size;
		if (!removed[which]) {
			entries[which].score = rand() % (size / 4);
			ttm_prio_add(&q, &entries[which]);
		}
	}

	// There better be the same amount.

	struct ttm_pqueue_entry *it = ttm_prio_query_lowest(&q);
	for (i = 0; it; it = ttm_prio_query_next(it)) i++;
	if (i != total) fail("Expected %u, got %u entries\n", total, i);

	return 0;
}
