#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crazy-prof.h"

void test1(){
  Professor();
  int i;
  for(i=0;i<1;i++) {
    Student(i, 3);    
  } 
  while (1);
  
}

void test2(){
  Professor();
  int i;
  for(i=0;i<1;i++) {
    Student(i, 10);   
  
  } 
  while (1);
  
}

void test3(){
  Professor();
  int i;
  for(i=0;i<10;i++) {
    Student(i, 1);       
  } 
  while (1);

}

void test4(){
  Professor();
  int i,j;
  for(j=0;j<5;j++) {
    for(i=0;i<2;i++) {
     
      Student(i+j*2, random()%10+1);  

    } 
    sleep(50);  
  }
  while (1);
 
}

void test5(){
  Professor();
  int i;
  for(i=0;i<100;i++) {
    Student(i, random()%10+1);
    usleep(random()%1000);   
  }
  while (1);

}

int main(int argc, char *argv[]){
	if( argc != 2 )
    	{
        	printf("Usage: ./crazy <selection (1 - 5) >:\n");
        	return 0;
    	}
	char *selection = argv[1];
	
	if (strcmp(selection,"1")==0) {
		test1();
	} else if (strcmp(selection,"2")==0) {
		test2();
	} else if (strcmp(selection,"3")==0) {
		test3();
	} else if (strcmp(selection,"4")==0) {
		test4();
	} else if (strcmp(selection,"5")==0) {
		test5();
	} else {
		printf("Usage: ./crazy <selection (1 - 5) >:\n");
		exit(1);
	}

	return 0;
	
	
}
