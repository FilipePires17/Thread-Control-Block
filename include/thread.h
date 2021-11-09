#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <threadu.h>
#include <queue.h>

#define NUMBER_OF_REGISTERS	15
#define STACK_SIZE		2048

void scheduler_entry();
void exit_handler();

typedef enum {
	      FIRST_TIME,
	      READY,
	      BLOCKED,
	      EXITED
} status_t;

typedef struct tcb {
	status_t status;
	uint64_t flags;
	uint64_t regs[NUMBER_OF_REGISTERS];
	uint64_t *sp;
	uint64_t stack[256];
	node_t   tcb_list;
} tcb_t;

#endif /* THREAD_H */