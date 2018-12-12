#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <pthread.h>

#include "ClientModule.h"

int main(){

	establishServerConnections();

	pthread_t t1, t2;

	pthread_create(&t1, NULL, createUserTask, NULL);
	pthread_create(&t2, NULL, getUserDataTask, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	system("pause");
	return 0;
}