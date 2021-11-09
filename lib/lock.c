#include <lock.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

node_t lock_queue;

enum {
      SPIN = FALSE,
};

// TODO: inicializes a lock
void lock_init(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		l->status = UNLOCKED;
		queue_init(&lock_queue);
	}
}

// TODO:  acquires a  lock if  it is  available or  blocks the  thread
// otherwise
void lock_acquire(lock_t * l)
{
	if (SPIN) {
		while (LOCKED == l->status)
			thread_yield();
		l->status = LOCKED;
	} else {
		if (l->status == UNLOCKED) {
			l->status = LOCKED;
		}
		else {
			block();
			thread_yield();
		}
	}
}

// TODO:  releases a  lock  and  unlocks one  thread  from the  lock's
// blocking list
void lock_release(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		unblock(l);
	}
}

// TODO: blocks the running thread
void block()
{
	node_t *tmp_node;
	tmp_node = my_malloc(sizeof(node_t));
	current_running->status = BLOCKED;
	tmp_node->item = current_running;
	enqueue(&lock_queue, tmp_node);
	return;
}

// TODO: unblocks  a thread that is waiting on a lock.
void unblock(lock_t *l)
{
	if (!is_empty(&lock_queue)) {
		node_t   *tmp_node;
		tmp_node = my_malloc(sizeof(node_t));
		tmp_node = dequeue(&lock_queue);
		((tcb_t*)(tmp_node->item))->status = READY;
		enqueue(&ready_queue, tmp_node);
	}
	else {
		l->status = UNLOCKED;
	}
}
