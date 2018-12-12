#define MAX 100

#include "ServerModule.h"

int count = 0, key = 1;

void *createUserTask(void *args){

	RequestObject *reqObj;
	
	while (count < 100){
		reqObj = (RequestObject *)malloc(sizeof(RequestObject));
		reqObj->key = count;
		reqObj->name = (char *)malloc(32 * sizeof(char));
		reqObj->name = "abc";
		reqObj->type = CREATE_USER;

		count += 1;
		enQueue(reqObj);

		Sleep(100);
	}
	return NULL;
}

void *getUserDataTask(void *args){
	
	RequestObject reqObj;
	
	while (key < 100){
		reqObj.key = key;
		reqObj.type = VIEW_USER;

		key += 1;
		enQueue(&reqObj);

		Sleep(100);
	}

	return NULL;
}