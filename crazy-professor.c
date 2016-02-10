#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "crazy-professor.h"

// Shared Variables
int studentID = 0; //The student ID who is asking the question
int questionNum = 0; // The question number from the student who is asking
int numStudents = 0; // Number of students waiting to ask questions
int quesAns = 0; // what is to be written on the board, 0 napping, 1 question, 2 answer.

// Condition Variables
pthread_cond_t atDoor;
pthread_cond_t isAvailable;
pthread_cond_t questionAsked;

// Locks
pthread_mutex_t plock; //Professor lock
pthread_mutex_t slock; //Student lock

//Initializes the mutex and the condition variables
void init(){

  pthread_mutex_init(&plock, NULL);
  pthread_mutex_init(&slock, NULL);

  pthread_cond_init(&atDoor, NULL);
  pthread_cond_init(&isAvailable, NULL);
  pthread_cond_init(&questionAsked, NULL);
	
}

/* Wait state for professor. Sets quesAns shared variable so student threads can ask questions when leaving the wait state.
*/
void Nap(){
	quesAns = 0;
	printf("Professor napping...\n");
	pthread_cond_wait(&atDoor, &plock);
	quesAns = 2; // set to answer state
	printf("Professor is awake!\n");
}

// Eat snack, creates a sleep state 
void Snack(){
	printf("Professor snacks on a cookie\n\n");
	usleep(500000);
}

// Changes 3 shared variables, student ID, question # and question state
void questionStart(int id, int questionNumber){
	printf("Student %d asks question #%d.\n", id, questionNumber);
	quesAns = 1; // Sets answer to question value.
	studentID = id;
	questionNum = questionNumber;
	usleep(random()%10000);	
}

// calculate the time it takes to run between time 1 and time 2
int timediff(struct timeval t2, struct timeval t1) {

  return ((t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec)) ; 
  
}

// The professor answers the question and also 
void answerQuestion(){
	struct timeval t1, t2;

	printf("Professor starts to answer question #%d for student %d\n", questionNum, studentID);
	gettimeofday(&t1, NULL);
	usleep(100 + random()%10000 );
	gettimeofday(&t2, NULL);
	printf("Professor is done with question #%d for student %d. Answering took %d microseconds.\n", questionNum, studentID, timediff(t2, t1));
	quesAns = 2;
}

void * Professor_start(void * p) {
  int cookieCounter = 0;
	

  while(1) {
 
    // If no students, then Nap
    // wait on the condition
    // that there is a student
    // at the door
    pthread_mutex_lock(&plock);
    while(numStudents==0) {
	  Nap();
    }
	//Signal students that he's now available
	pthread_cond_broadcast(&isAvailable);

    // Wait for student to ask the question
	while(quesAns != 1)
		pthread_cond_wait(&questionAsked, &plock);
    
    // Answer a student's question
	answerQuestion();
	cookieCounter++;

    // If done answering 3 questions: Eat a cookie
	if(cookieCounter%3 == 0)
	   Snack();
    
    // Notify availability
    pthread_mutex_unlock(&plock);
	pthread_cond_broadcast(&isAvailable);

  }

  pthread_exit(NULL);

  return NULL;
  
}


void * Student_start(void * s) {

  studentStruct* myStudent = (studentStruct *) s;

  int id =  myStudent -> ID;
  int numQuestions = myStudent -> numQues;
  int questionNumber = 1;

  // Increase the student counter
  pthread_mutex_lock(&slock);
  numStudents++;
  pthread_mutex_unlock(&slock);
  // Keep knocking on door
  while(quesAns == 0)
	  pthread_cond_signal(&atDoor);
  
  while(numQuestions) {

    // Wait for Professor to become available, and that no other student has asked a question
	pthread_mutex_lock(&slock);
	while(quesAns <= 1 )    
    	pthread_cond_wait(&isAvailable, &slock); 
    // Ask the next question
	//sleep(1);
	questionStart(id, questionNumber);
	questionNumber++; //Now that question has been asked, prep for next question
	pthread_cond_signal(&questionAsked);

    // Notify professor that question
    // has been asked
    pthread_mutex_unlock(&slock);
    numQuestions--;
  }

  pthread_mutex_lock(&slock);
  numStudents--;
  pthread_mutex_unlock(&slock);
  

  // pthread_exit(NULL);

  return NULL;

}

// Create the Student thread that runs the Student_start
void Student(int id, int numQuestions) {

	pthread_t sthread;	
	
	studentStruct * s;
	s = (studentStruct *)malloc(sizeof(studentStruct));
	s -> ID = id;
	s -> numQues = numQuestions;

   if(pthread_create(&sthread, NULL, Student_start, (void *) s) !=0 ) {
      perror("Student creation failed!\n");
      exit(0);
    }
}

// Create the Professor thread that runs the Professor_start
void Professor() {
	
	init();
	pthread_t profthread;

   if(pthread_create(&profthread, NULL, Professor_start, NULL )) {
      perror("Professor creation failed!\n");
      exit(0);
    }
}
