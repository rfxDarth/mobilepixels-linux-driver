/*
 * Thread-Safe Queue
 *
 * queue.h
 *
 * A Thread-Safe FIFO queue implementation using double linked lists
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

queue_t* queue_create() {
	queue_t *queue;
	queue = malloc(sizeof(queue_t));
	queue->first = NULL;
	queue->last = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

void releses_queue(queue_t* queue)
{
 	DEBUG_PRINT("%s \n",__func__);
	//while(queue_remove(queue) != NULL);
	char* request = NULL; 
	do{
		pthread_mutex_lock(&queue->mutex);
		DEBUG_PRINT("[queue_destroy] The queue is not empty=%d\n",queue->length);
		if (queue->length > 0) {
			pthread_mutex_unlock(&queue->mutex);
			request = queue_remove(queue);
			if(request != NULL)	{
				free(request);
			}
			continue;
		}else{
			pthread_mutex_unlock(&queue->mutex);
			break;
		}
	}while(1);	
	
	
}

void queue_destroy(queue_t* queue) {

	DEBUG_PRINT("%s \n",__func__);
	if (queue == NULL) {
		DEBUG_PRINT("[queue_destroy] Tried to destroy a 'NULL' queue\n");
		return;
	}
	
	pthread_mutex_destroy(&queue->mutex);
	free(queue);
	queue = NULL;
	DEBUG_PRINT("queue_destroy \n");
}

void queue_add(queue_t* queue, void* element) {
	queue_element_t* new_element;
	if (queue == NULL || element == NULL) {
		DEBUG_PRINT("[queue_add] Invalid queue or element\n");
		return;
	}
	pthread_mutex_lock(&queue->mutex);
	new_element = malloc(sizeof(queue_element_t));
	new_element->element = element;
	new_element->next = NULL;
	if (queue->first == NULL) {
		queue->first = new_element;
		queue->last = new_element;
	}
	else {
		queue->last->next = new_element;
		queue->last = new_element;
	}
	queue->length++;
	pthread_mutex_unlock(&queue->mutex);
}

size_t queue_length(queue_t* queue) {
	return queue->length;
}

void* queue_first(queue_t *queue) {
	pthread_mutex_lock(&queue->mutex);
	if ((queue == NULL) || (queue->first == NULL)) {
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	pthread_mutex_unlock(&queue->mutex);
	return queue->first->element;
}

void* queue_remove(queue_t* queue) {
	queue_element_t* node;
	pthread_mutex_lock(&queue->mutex);
	if ((queue == NULL) || (queue->first == NULL)) {
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	node = queue->first;
	if (queue->first == queue->last) {
		queue->first = NULL;
		queue->last = NULL;
	}
	else {
		queue->first = node->next;
	}
	queue->length--;
	void* element = node->element;
	free(node);
	pthread_mutex_unlock(&queue->mutex);
	return element;
}

