/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <stdio.h>
#include <stdlib.h>
#include "vmm.h"



struct queue *newqueue(void)
{
	struct queue *newQueue = (struct queue*) malloc(sizeof(struct queue));

	newQueue->head = NULL;
	newQueue->tail = NULL;
	newQueue->size = 0;

	return newQueue;
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
int	isempty(struct queue *q)
{
	return (q->size == 0);
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
int	nonempty(struct queue *q)
{
	return (q == NULL || q->size > 0);
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
int	isfull(struct queue *q)
{
	return (q->size >= num_frames);
}

/**
 * Insert a process into a queue in descending key order
 * @param key	key of process to insert
 * @param q	Pointer to the queue to use
 */
fte  enqueue(fte key, struct queue *q)
{
        if (isfull(q)) {
                return SYSERR;
        }

	//create entry
	struct qentry *newEntry = (struct qentry*) malloc(sizeof(struct qentry));
	newEntry->key = key;
	newEntry->next = NULL;
	newEntry->prev = NULL;

	//find insert location
	struct qentry *currEntry;	// runs through items in a queue
	struct qentry *prevEntry;	// holds previous entry

	currEntry = q->head;	
	if (currEntry == NULL) {		//goes in the tail of the queue
		prevEntry = q->tail;
	}
	else {
		prevEntry = currEntry->prev;
	}

	// insert process between curr node and previous node
	newEntry->prev = prevEntry;
	newEntry->next = currEntry;

	if (prevEntry != NULL) {
		prevEntry->next = newEntry;
	}
	if (currEntry != NULL) {
		currEntry->prev = newEntry;
	}

	// update queue
	if (NULL == prevEntry) {	//new head
		q->head = newEntry;
	}
	if (NULL == currEntry)	{ //new tail
		q->tail = newEntry;
	}
	q->size++;			//increment size

       	return 1;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return key of the process removed, or EMPTY if queue is empty
 */
fte dequeue(struct queue *q)
{
	fte	key;	//ID of process removed

	if (isempty(q)) {
		return EMPTY;
	}

	//save pointer to head entry
	struct qentry *head = q->head;
	struct qentry *newHead = head->next;

	//save key of head entry
	key = head->key;

	//unlink head from queue
	if (newHead != NULL) {
		newHead->prev = NULL;
	}
	else {
		q->tail = NULL;
	}

	//update queue to point head pointer at newhead
	q->head = newHead;

	//decrement size of queue
	q->size--;

	//deallocate head entry
	free(head);

	return key;
}


/**
 * Finds a queue entry given key
 * @param key	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbykey(fte key, struct queue *q)
{
	if (isempty(q)) {
		return NULL;
	}

	struct qentry *currEntry = q->head;
	while(currEntry != NULL && currEntry->key != key) {
		currEntry = currEntry->next;
	}

	return currEntry;
}

/**
 * Remove a process from the front of a queue (key assumed valid with no check)
 * @param q	pointer to a queue
 * @return key on success, EMPTY if queue is empty
 */
fte	getfirst(struct queue *q)
{
	if (isempty(q)) {
		return EMPTY;
	}
	return dequeue(q);
}


/**
 * Remove a process from the end of a queue (key assumed valid with no check)
 * @param q	Pointer to the queue
 * @return key on success, EMPTY otherwise
 */
fte	getlast(struct queue *q)
{
	if (isempty(q)) {
		return EMPTY;
	}
	return q_remove(q->tail->key, q);
}


/**
 * Remove a process from an arbitrary point in a queue
 * @param key	key of process to remove
 * @param q	Pointer to the queue
 * @return key on success, SYSERR if key is not found
 */
fte	q_remove(fte key, struct queue *q)
{
	if (isempty(q)) {
		return EMPTY;
	}
	
	//find the entry with key
	struct qentry *currEntry = q->head;
	while (currEntry != NULL) {
		//found it!
		if (currEntry->key == key) {
			//unlink: find next and prev entries
			struct qentry *next = currEntry->next;
			struct qentry *prev = currEntry->prev;
			if (next != NULL) {
				next->prev = prev;
			}
			if (prev != NULL) {
				prev->next = next;
			}

			//update queue structure
			if (currEntry == q->head) {
				q->head = next;
			}
			if (currEntry == q->tail) {
				q->tail = prev;
			}
			q->size--;

			//deallocate current entry
			free(currEntry);
			return key;
		}
		currEntry = currEntry->next;
	}

	return SYSERR;
}
