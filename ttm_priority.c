/**************************************************************************
 *
 * Copyright (c) 2014 Lauri Kasanen
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#include "list.h"

#define container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

typedef unsigned long u64;

struct entry {
	unsigned long score;
	char value[10];

	struct list_head list;
	struct rb_node node;
};

static void ttm_prio_insert_rb(struct rb_root *root, struct entry *data)
{
	struct rb_node **new = &root->rb_node, *parent = NULL;

	while (*new) {
		struct entry *this = container_of(*new, struct entry, node);
		parent = *new;

		if (data->score < this->score)
			new = &((*new)->rb_left);
		else if (data->score > this->score)
			new = &((*new)->rb_right);
		else
			return; // bug_on fixme
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);
}

static struct entry *ttm_prio_search_rb(struct rb_root *root, unsigned long score)
{
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

void ttm_prio_add(struct rb_root * const tree, struct entry * const entry)
{
	struct entry *test = ttm_prio_search_rb(tree, entry->score);

	if (!test)
		ttm_prio_insert_rb(tree, entry);
	else
		list_add_tail(&entry->list, &test->list);
}

struct entry *ttm_prio_query_lowest(const struct rb_root * const root)
{
	struct rb_node *node;

	node = rb_first(root);
	if (!node)
		return NULL;

	return container_of(node, struct entry, node);
}

void ttm_prio_remove(struct rb_root * const tree, struct entry * const entry)
{
	if (list_empty(&entry->list)) {
		rb_erase(&entry->node, tree);
	} else {
		struct entry *next = list_first_entry(&entry->list, struct entry, list);

		rb_replace_node(&entry->node, &next->node, tree);
		list_del_init(&entry->list);
	}
}
