#include <lock.h>
#include <threadu.h>
#include <thread.h>
#include <queue.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

node_t lock_queue;

enum {
      SPIN = TRUE,
};

// TODO: inicializes a lock
void lock_init(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		l->status = UNLOCKED;
		l->tid = 0;
		queue_init(&lock_queue);
		(&lock_queue)->item = l;
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
	lock_t *tmp_lock;
	tmp_lock = my_malloc(sizeof(lock_t));
	tmp_lock = (lock_t*) (&lock_queue)->item;
	if (tmp_lock->status == LOCKED) {
		node_t new_lock;
		tmp_lock = (&new_lock)->item;
	}
}

// TODO: unblocks  a thread that is waiting on a lock.
void unblock(lock_t *l)
{
	if (!is_empty(&lock_queue)) {
		node_t   *tmp_node;
		//thread_t *tmp_thread;
		tmp_node = my_malloc(sizeof(node_t));
		tmp_node = dequeue(&lock_queue);
		((tcb_t*)((thread_t*) tmp_node->item)->tcb)->status = READY;
	}
	else {
		l->status = UNLOCKED;
	}
}
