struct Node{
	char *title;
	float cost;
	int id;
	char *category;
	struct Node* next;
};
typedef struct Node* NodePtr;

struct Queue{
	NodePtr head;	
	NodePtr tail;
	NodePtr list;
};
typedef struct Queue* QueuePtr;

QueuePtr createQueue();

void enqueue(char *title, float cost, int id, char *category, QueuePtr q);

NodePtr dequeue(QueuePtr q);

void clearQueue(QueuePtr q);

NodePtr peek(QueuePtr q);

int isEmpty(QueuePtr q);
