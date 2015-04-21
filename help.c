#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char ** parse(char * pth){
	char * path = malloc(sizeof(char)*strlen(pth)+1);
	path = strcpy(path,pth);
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

int getFunction(char * func){
	int i;
	for(i = 1; i < 8; i++){
		if(strcmp(func, functions[i]) == 0)
			return i;
	}//end of what ufnciton isit 
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
		printf("we're adding %d and %d!\n", fpara1, fpara2);
		printf("%s and %s\n", para1, para2);
		ret = fpara1 + fpara2;

	}//end of add

	return ret;
}//end of domath


int main(){
	char * p = "/add/3/4";
	char ** ret = parse(p);
	int i;
	for(i = 0; i < 3; i ++){
		printf("ret[something] %s\n",ret[i]);
		printf(".ret[something] %s\n",ret[i]);
	}

	int func = getFunction(ret[0]);
	printf("func is %s\n",functions[func]);

	//char * one = (char *)malloc(sizeof(char) * strlen(ret[1])
	printf("strlen of 1 is %d and strlen of 2 is %d\n", strlen(ret[1]), strlen(ret[2]));
	printf("strlen of 1 is %d and strlen of 2 is %d\n", strlen(ret[1]), strlen(ret[2]));

	double boom = doMath(func, ret[1], ret[2]);
	printf("BOOM: %f\n",boom);

}//end of main
