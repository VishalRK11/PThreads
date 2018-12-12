typedef struct RequestObject{
	int type;
	int key;
	char *name;
	int status;
}RequestObject;

typedef struct ThreadData{
	int status;
	RequestObject *reqObj;
}ThreadData;

typedef struct Queue{
	RequestObject *reqObj;
	Queue *next;
}Queue;

typedef struct ConcurrentQueue{
	ThreadData thread[4];
	Queue *head;
	Queue *tail;
}ConcurrentQueue;

typedef struct Database{
	int key;
	char name[32];
}Database;
