struct Node{
	char *title;
	int cost;
	int id;
	char *category;
	struct Node* next;
}
typedef struct Node* NodePtr;

struct Queue{
	NodePtr head;	
	NodePtr tail;
	NodePtr list;
}
typedef struct Queue* QueuePtr;

QueuePtr createQueue();

int enqueue(char *title, int cost, int id, char *category, QueuePtr q);

NodePtr dequeue();

void clearQueue(QueuePtr q);

NodePtr peek(QueuePtr q);


