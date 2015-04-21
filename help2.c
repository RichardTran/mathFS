
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

static const char* functions[]={
	"/",    	//0
	"/add", 	//1
	"/sub", 	//2
	"/mult",	//3
	"/div", 	//4
	"/factor",	//5
	"/exp", 	//6
	"/fib",  	//7
	};

char ** parse(const char * pth){
	char * path = malloc(sizeof(char)*strlen(pth)+1);
	path = strcpy(path,pth); //have to copy as path name is given as const
	char ** ptr = malloc(sizeof(char*)*3); //THIS WAS THE PROBLEM. YOU WERE RIGHT. 
						//MY BAD. WE HAD TO MALLOC
	char c;
	int counter = 1;
	int i;
	int length = strlen(path);

	for(i = 0 ; i < 3; i++){
		*(ptr+i) = NULL;
		printf("LINE: %d\n", __LINE__);
	}

	ptr[0] = path;

	for(i = 1; i < length; i ++){
		c = *(path+i);
		if(c=='/'){
			//path[i] = '\0';
			*(path+i) = '\0';
			ptr[counter] = (char *)(path+i+1);
			counter++;
		}//end of if it's a / 
		else{
			continue;
		}
	}//end of for 
	return ptr;

}//end of parse

//check if valid inputs
int isNumber(char* numberString){
	if(numberString==NULL){
		return 0;
	}
	int i;
	char c;
	int decFlag = 0; // used to check if there's a decimal. Only allow at most 1 decimal.
	for(i = 0; i < strlen(numberString); i++){
		c = *(numberString+i);
		if(isdigit(c) || (c == '.' && decFlag == 0)){
			if(c=='.'){
				decFlag++;
			}
		}
		else{
			return 0; //failure
		}
	}
	return 1; //success
}//end of isNumber

int getFunction(char** ptr){
	int i;
	for(i = 1; i < 8; i++){
		if(strcmp(ptr[0], functions[i]) == 0){
			if( ((i >= 1 && i <= 4) || i==6) && isNumber(ptr[1]) && isNumber(ptr[2]) ){
				return i;
			}
			else if( (i == 5 || i == 7) && isNumber(ptr[1]) && ptr[2]==NULL){
				return i;
			}
		}
	}
	return -1; //error
}//end of getFunction

double doMath(int fundex, char * para1, char*  para2){
	char* a = malloc(strlen(para1)+1);
	char* b = malloc(strlen(para2)+1);
	a = strcpy(a,para1);
	b = strcpy(b,para2);
	double fpara1 = atof(a);
	double fpara2 = atof(b);
	double ret = -1;

	if(fundex == 1){
		printf("we're adding %f and %f!\n", fpara1, fpara2);
		printf("%s and %s\n", para1, para2);
		ret = fpara1 + fpara2;

	}//end of add

	return ret;
}//end of domath

void freePtr(char** ptr){
	if(ptr!=NULL){
		if(ptr[0]!=NULL){
			free(ptr[0]);
		}
		if(ptr[1]!=NULL){
			free(ptr[1]);
		}
		if(ptr[2]!=NULL){
			free(ptr[2]);
		}
		free(ptr);
	}	
}
