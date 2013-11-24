#include "queue.h"

QueuePtr createQueue(){
	QueuePtr q;
	q = malloc(sizeof(struct Queue));
	return q;
}

void enqueue(char *title, int cost, int id, char *category, QueuePtr q){
	if(q->head == q->tail == q->list == NULL){
		q->list = malloc(sizeof(struct Node));
		q->head = q->tail = q->list;
		q->list->title = malloc(sizeof(title));
		strcpy(q->list->title, title);
		q->list->category = malloc(sizeof(category));
		strcpy(q->list->category, category);
		q->list->id = id;
		q->list->cost = cost;
		q->list-next = malloc(sizeof(struct Node));
		q->tail = q->tail->next;
		return;
	}
	q->tail->title = malloc(sizeof(title));
	strcpy(q->list->title, title);
	q->tail->category = malloc(sizeof(category));
	strcpy(q->list->category, category);
	q->tail->id = id;
	q->tail->cost = cost;
	q->tail->next = malloc(sizeof(struct(Node));
	q->tail = q->tail->next;
	return;
}

NodePtr dequeue(){
	NodePtr info = NULL;
	if(q-head == q->tail == NULL){
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
	NodePtr temp = q->head;
	while(temp != NULL){
		temp = q->head;
		q->head = q->head->next;
		free(temp->title);
		free(temp->category);
		free(temp);
		temp = q->head;
	}
	free(q);
}

NodePtr peek(QueuePtr q){
	if(q->head != NULL){
		return q->head
	}
	return NULL;
}
