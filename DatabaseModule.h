#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "Structures.h"

Database records[100];
int rows = 0;

int insertUser(int key, char *name){
	for (int i = 0; i < rows; i++){
		if (key == records[i].key){
			return -1;
		}
	}
	records[rows].key = key;
	strcpy(records[rows].name, name);
	rows++;
	return 1;
}

char *getData(int key){
	char *uName = (char *)calloc(1, sizeof(char));
	for (int i = 0; i < rows; i++){
		if (key == records[i].key){
			strcpy(uName, records[i].name);
			return uName;
		}
	}
	return NULL;
}