#include <stdlib.h>
#include <string.h>
#include "queue.h"

QueuePtr createQueue(){
	QueuePtr q;
	q = malloc(sizeof(struct Queue));
	q->head = q->tail = q->list = NULL;
	return q;
}

void enqueue(char *title, float cost, int id, char *category, QueuePtr q){
	if(q->head == NULL && q->tail == NULL && q->list == NULL){
		q->list = malloc(sizeof(struct Node));
		q->head = q->list;
		q->list->title = title;
		q->list->category = category;
		q->list->id = id;
		q->list->cost = cost;
		q->list->next = malloc(sizeof(struct Node));
		q->tail = q->list->next;
		return;
	}
	q->tail->title = title;
	q->tail->category = category;
	q->tail->id = id;
	q->tail->cost = cost;
	q->tail->next = malloc(sizeof(struct Node));
	q->tail = q->tail->next;
	return;
}

NodePtr dequeue(QueuePtr q){
	NodePtr info = NULL;
	if(q->head == NULL && q->tail == NULL){
		return info;
	}
	if(q->head == q->tail){			
		info = q->head;
		q->head = q->tail = NULL;
		return info;
	}
	info = q->head;	
	q->head = q->head->next;
	return info;
}

void clearQueue(QueuePtr q){
	NodePtr temp, temp2;
	temp = temp2 = q->list;
	while(temp2 != NULL){
		temp2 = temp->next;
		free(temp);
		temp = temp2;
	}
	free(q);
}

NodePtr peek(QueuePtr q){
	if(q->head != NULL){
		return q->head;
	}
	return NULL;
}

int isEmpty(QueuePtr q){
	if(q->head == NULL || q->head->title == NULL)
		return 1;
	return 0;	
}
