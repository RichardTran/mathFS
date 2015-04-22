
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
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

int * getFactors(int n){
	int counter = 2;
	int * factors = (int *)malloc(sizeof(int)*counter);
	while(n%2 == 0){
		counter++;
		factors = realloc(factors,sizeof(int)* counter);
		printf("%d\t",2);
		factors[counter-3] = 2;
		n = n/2;
	}
	
	int i;
	for(i = 3; i < sqrt(n); i = i+2){
		while(n % i == 0){
			counter++;
			factors = realloc(factors,sizeof(int)*counter);
			printf("%d\t",i);
			factors[counter-3] = i;
			n = n/i;
		}//end of while 
	}//end off or 

	if(n > 2){
		counter++;
		factors = realloc(factors, sizeof(int)*counter);
		printf("%d\t\n",n);
		factors[counter-3] = n;
	}//end of if 

	return factors;
}//end of getFactors 

int getFib(int n){
	if(n==0)
		return 0;
	else if(n==1)
		return 1;
	else
		return (getFib(n-1) + getFib(n-2));
}//end of getFib

double * doMath(int fundex, char * para1, char*  para2){
	char* a = malloc(strlen(para1)+1);
	char* b = malloc(strlen(para2)+1);
	a = strcpy(a,para1);
	b = strcpy(b,para2);
	double fpara1 = atof(a);
	double fpara2 = atof(b);
	double * ret = NULL;

	if(fundex == 1){
		ret = (double *)malloc(sizeof(double)*1);
		printf("we're adding %f and %f!\n", fpara1, fpara2);
		ret[0] = fpara1 + fpara2;
	}//end of add
	else if(fundex == 2){
		ret = (double *)malloc(sizeof(double)*1);
		printf("we're subtracting %f and %f!\n",fpara1, fpara2);
		ret[0] = fpara1 - fpara2;
	}//end of subtract
	else if(fundex == 3){
		ret = (double *)malloc(sizeof(double)*1);
		printf("we're multiplying %f and %f!\n",fpara1, fpara2);
		ret[0] = fpara1 * fpara2;
	}//end of multiply
	else if(fundex == 4){
		ret = (double *)malloc(sizeof(double)*1);
		printf("we're dividing %f and %f!\n",fpara1, fpara2);
		ret[0] = fpara1 / fpara2;
	}//end of divide		
	else if(fundex == 5){
		printf("we're computing the prime factors for %f\n",fpara1);
	}//end of factor
	else if(fundex == 6){
		ret = (double *)malloc(sizeof(double)*1);
		printf("we're raising %f to the %f\n",fpara1, fpara2);
		ret[0] = (double)pow(fpara1, fpara2);
	}//end of exponent
	else if(fundex == 7){
		printf("LINE %d\n", __LINE__);
		printf("we're getting the %fth fib number\n",fpara1);
		int i;
		double * fibs = (double *)malloc(sizeof(double)*(int)fpara1);
		for(i = 0; i < (int)fpara1; i++){
			fibs[i] = (double)getFib(i);	
		}//end of for 
		ret = fibs;
		for(i = 0; i < (int)fpara1; i++){
			printf("ret[%d] is %f\n",i,ret[i]);
		}//end of for 
	}//end of fibonacci
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

int main(){
	char * p = "/factor/138";
	char ** ret = parse(p);
	int i;
	/*
	for(i = 0; i < 3; i ++){
		printf("ret[something] %s\n",ret[i]);
	}
	*/

	int func = getFunction(ret);
	printf("func is %s\n",functions[func]);


	if(ret[2] == NULL){
		ret[2] = "0";
	} // end of if there is only 1 parameter 

	double * boom = doMath(func, ret[1], ret[2]);
	printf("sizeof is %d\n", sizeof(boom)/sizeof(boom[0]));

	for(i = 0; i < sizeof(boom)/sizeof(boom[0]) +1;i++){
		printf("BOOM[%d] is %f\n",i,boom[i]);
	}//end of for 
	
	/*
	for(i = 0; i < atoi(ret[1]); i++){
		printf("BOOM[%d] is %f\n",i,boom[i]);
	}//end of for 

	int * facts = getFactors(138);
	printf("facts sizeof is %d\n",sizeof(facts)/sizeof(facts[0]));
	for(i =0 ; i < sizeof(facts)/sizeof(facts[0])+1; i++){
		printf("FACTS[%d] is %f\n",i,(double)facts[i]);
	}//end of for
	*/


}//end of main
