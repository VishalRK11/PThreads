#define FREE 1
#define BUSY 2
#define CREATE_USER 1
#define VIEW_USER 2

#include <pthread.h>
#include "DatabaseModule.h"

ConcurrentQueue cQueue;

pthread_rwlock_t rwLock = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t rwLockInsert = PTHREAD_RWLOCK_INITIALIZER;

int isQueueEmpty(){
	if (cQueue.head == NULL)
		return -1;
	return 1;
}

void enQueue(RequestObject *reqObj){
	pthread_rwlock_wrlock(&rwLockInsert);
	printf("\nKey: %d\nType: %15d\n\n", reqObj->key, reqObj->type);

	if (cQueue.head == NULL){
		cQueue.head = (Queue *)malloc(sizeof(Queue));
		cQueue.head->reqObj = reqObj;
		cQueue.head->next = NULL;
		cQueue.tail = cQueue.head;
	}
	cQueue.tail->next = (struct Queue *)calloc(1, sizeof(struct Queue));
	cQueue.tail = cQueue.tail->next;
	cQueue.tail->reqObj = reqObj;
	cQueue.tail->next = NULL;

	pthread_rwlock_unlock(&rwLockInsert);
}

int getFreeThread(){
	for (int index = 0; index < 4; ++index)
		if (cQueue.thread[index].status == FREE)
			return index;
	return -1;
}

void *scheduler(void *args){
	while (1){
		pthread_rwlock_wrlock(&rwLockInsert);
		
		int emptyFlag = isQueueEmpty();
		if (emptyFlag != -1){
			int threadIndex = getFreeThread();
			if (threadIndex != -1){
				cQueue.thread[threadIndex].reqObj = cQueue.head->reqObj;
				struct Queue *temp = cQueue.head;
				cQueue.head = cQueue.head->next;
				free(temp);
			}
		}
		pthread_rwlock_unlock(&rwLockInsert);
	}
	return NULL;
}

void *service(void *args){
	
	int index = *(int *)args;
	
	while (1){
	
		if (cQueue.thread[index].reqObj != NULL){
			
			cQueue.thread[index].status = BUSY;
			printf("\nThread %d\nKey: %d\nTask: %d\n\n", index, cQueue.thread[index].reqObj->key, cQueue.thread[index].reqObj->type);
		
			if (cQueue.thread[index].reqObj->type == CREATE_USER){
			
				pthread_rwlock_wrlock(&rwLock);
				cQueue.thread[index].reqObj->status = insertUser(cQueue.thread[index].reqObj->key, cQueue.thread[index].reqObj->name);
				
				if (cQueue.thread[index].reqObj->status == -1)
					printf("\nTask %d: Key already exists!\n\n", cQueue.thread[index].reqObj->key);
				else
					printf("\nTask %d: Record inserted successfully!\n\n", cQueue.thread[index].reqObj->key);
				
				pthread_rwlock_unlock(&rwLock);
			}
			else{
				
				pthread_rwlock_rdlock(&rwLock);
				cQueue.thread[index].reqObj->name = getData(cQueue.thread[index].reqObj->key);
				
				if (cQueue.thread[index].reqObj->name == NULL)
					printf("\nTask %d: Key doesn't exist!\n\n", cQueue.thread[index].reqObj->key);
				else
					printf("\Task %d: Name is %s\n\n", cQueue.thread[index].reqObj->key, cQueue.thread[index].reqObj->name);
				
				pthread_rwlock_unlock(&rwLock);
			}
		
			cQueue.thread[index].reqObj = NULL;
			cQueue.thread[index].status = FREE;
		}
	}
	return NULL;
}

void createThreads(){
	pthread_t threads[4];
	int *j;
	for (int i = 0; i < 4; i++){
		j = (int *)malloc(sizeof(int));
		j[0] = i;
		pthread_create(&threads[i], NULL, service, (void *)j);
		cQueue.thread[i].status = FREE;
		cQueue.thread[i].reqObj = NULL;
	}
}

void establishServerConnections(){
	
	pthread_t schedulerThread;

	createThreads();

	pthread_create(&schedulerThread, NULL, scheduler, NULL);
}