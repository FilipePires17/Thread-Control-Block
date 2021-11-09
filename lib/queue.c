#include <queue.h>

/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// TODO: inicializes a queue
void queue_init(node_t * queue)
{
  queue->next = queue;
  queue->prev = queue;
}

// TODO: returns the first element of the queue
node_t *dequeue(node_t * queue)
{
  if (queue->next != queue) {
    node_t *aux;
    aux = queue->next;
    aux->next->prev = queue;
    queue->next = aux->next;
    return aux;
  }
	return NULL;
}

// TODO: inserts a node in a queue
void enqueue(node_t * queue, node_t * item)
{
  item->prev = queue->prev;
  item->next = queue;
  item->prev->next = item;
  item->next->prev = item;
}

// TODO: checks if a queue is empty
int is_empty(node_t *queue)
{
  if (queue->next == queue) {
    return 1;
  }
	return 0;
}

node_t *peek(node_t *queue)
{
  if (queue->next != queue) {
    return queue->next;
  }
  return NULL;
}

int counts_elements(node_t *queue)
{
  int counter = 0;
  node_t *item;
  item = queue->next;
  while (item != queue) {
    counter++;
    item = item->next;
  }

  return counter;
}