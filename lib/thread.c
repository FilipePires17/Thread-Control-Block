#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thread.h>

node_t	ready_queue;
node_t  *current_node;
tcb_t	*current_running;

int tid_global = 0;

void *my_malloc(int size)
{
	void *tmp = malloc(size);
	if (tmp == NULL) {
		fprintf(stderr, "Could not alloc memory\n");
		exit(1);
	}
	memset(tmp, 0, size);
	return tmp;
}

/*
  thread_init: initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
 */
int thread_init()
{
	node_t 	*main_node;
	tcb_t 	*main_tcb;

	main_node = (node_t*) my_malloc(sizeof(node_t));
	main_tcb = (tcb_t*) my_malloc(sizeof(tcb_t));
	main_tcb->sp = main_tcb->stack + (STACK_SIZE/8) - 1;
	main_tcb->status = READY;
	queue_init(&ready_queue);
	
	current_running = main_tcb;
	main_node->item = current_running;
	current_node = main_node;
	return 0;
}

// creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{
	tcb_t  *tcb;
	node_t *node;

	tcb = (tcb_t*) my_malloc(sizeof(tcb_t));
	tcb->status = READY;
	tcb->sp = &(tcb->stack[255]);
	tcb->flags = (u_int64_t) 0;
	for (int i = 0; i<NUMBER_OF_REGISTERS; i++) {
		tcb->regs[i] = (u_int64_t) 0;
	}
	tcb->regs[9] = (u_int64_t) arg;
	*(tcb->sp) = (u_int64_t) start_routine;

	thread->tcb = tcb;

	node = (node_t*) my_malloc(sizeof(node_t));
	node->item = thread->tcb;

	enqueue(&ready_queue, node);

	return 0;
}

// TODO: yields the CPU to another thread
int thread_yield()
{
	scheduler_entry();
	return 0;
}

// TODO: waits for a thread to finish
int thread_join(thread_t *thread, int *retval) 
{

	while (((tcb_t*) thread->tcb)->status != EXITED) {
		thread_yield();
	}

	*retval = (int) ((tcb_t*) thread->tcb)->regs[14];
	//free(thread->tcb);
	return 0;
}

// TODO: marks a thread as EXITED and terminates the thread
void thread_exit(int status)
{
	current_running->status = EXITED;
	current_running->regs[14] = status;

	thread_yield();
	return;
}

// TODO: selects the next thread to execute
void scheduler()
{
	if (current_running->status == READY)
		enqueue(&ready_queue, current_node);
	
	if (!is_empty(&ready_queue)) {
		current_node = dequeue(&ready_queue);
		current_running = (tcb_t*) current_node->item;
	}
		
	return;
}

// TODO: you must  make sure this function is called  if a thread does
// not call thread_exit
void exit_handler()
{
}

