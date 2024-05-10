/*
 * Thread-Safe Queue
 *
 * queue.h
 *
 * A Thread-Safe FIFO implementation
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <pthread.h>

/*
 * Queue data structures
 */

typedef struct queue_element {
	void* element;
	struct queue_element *next;
} queue_element_t;

typedef struct {
	queue_element_t *first;
	queue_element_t *last;
	size_t length;
	pthread_mutex_t mutex;
} queue_t;


/*
 * queue_create: creates and initializes a new queue
 *   returns: pointer to the newly created queue
 */
queue_t* queue_create();

/*
 * queue_destroy: destroys a queue and frees all allocated memory
 * - queue: the queue to destroy
 */
void queue_destroy(queue_t *queue);
void releses_queue(queue_t* queue);

/*
 * queue_length: returns queue length
 * - queue: the queue to examine
 *   returns: the size of the queue
 */
size_t queue_length(queue_t *queue);

/*
 * queue_first: returns a pointer to the first element in the queue
 * - queue: the queue to examine
 *   returns: pointer to the first element, NULL if empty
 */
void* queue_first(queue_t *queue);

/*
 * queue_add: inserts a new element on the end of the queue
 * - queue: the queue where to insert the element
 * - element: the element to be inserted
 */
void queue_add(queue_t *queue, void* element);

/*
 * queue_remove: removes the first element of the queue
 * - queue: the queue where to remove the element from
 *   returns: pointer to the first element, NULL if empty
 */
void* queue_remove(queue_t *queue);



#endif
