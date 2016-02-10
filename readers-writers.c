#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define READERS 10
#define WRITERS 4

long i;
pthread_t r[READERS];
pthread_t w[WRITERS];

long shared = 999;

pthread_mutex_t wlock;
pthread_mutex_t rclock;

int reader_count = 0;


// What the writer does
void * writer(void * index) {

  long w = (long)index;

  while(1) {
  
    // ENTRY SECTION
    pthread_mutex_lock(&wlock);
  

    // CRITICAL SECTION
    shared = random() % 1000;
    printf("Writer[%ld] wrote: %ld\n", w, shared);
  

    // EXIT SECTION
    pthread_mutex_unlock(&wlock);

    // Do a lot of useful things here which consume time
  
  }
  // if you want to return a value
  // to a joining thread you would
  // use:
  pthread_exit(NULL);

  return NULL;
}



// What the reader does
void * reader(void * index) {

  long r = (long)index;

  while(1) {
   
    // ENTRY SECTION
    pthread_mutex_lock(&rclock);
    if(reader_count==0) {
      pthread_mutex_lock(&wlock);
    }
    reader_count++;
    pthread_mutex_unlock(&rclock);


    // CRITICAL SECTION
    printf("Reader[%ld] read: %ld\n", r, shared);

    
    // EXIT SECTION
    pthread_mutex_lock(&rclock);
    reader_count--;
    if(reader_count==0) {
      pthread_mutex_unlock(&wlock);
    }
    pthread_mutex_unlock(&rclock);

    

    // DO A LOT OF USEFUL
    // THINGS HERE!
    usleep(random()%10000);


  }

  

  return NULL;
}

void WRITER(long i) {

   if(pthread_create(&w[i], NULL, writer, (void *)i)!=0) {
      perror("Writer creation failed!");
      exit(0);
    }
   
}

void READER(long i) {
   if(pthread_create(&r[i], NULL, reader, (void *)i)!=0) {
      perror("Reader creation failed!");
      exit(0);
    }
}

int main (int argc, char *argv []) {
  
  pthread_mutex_init(&wlock, NULL);

  // create the writers
  for(i=0; i < WRITERS; i++) {
    WRITER(i);
  }
    
  // create the readers
  for(i=0; i < READERS; i++) {
    READER(i);
  }

  // join the writers
  for(i=0; i < WRITERS; i++) {
    pthread_join(w[i], NULL);
  }
 
  // join the readers
  for(i=0; i < READERS; i++) {
    pthread_join(r[i], NULL); 
  }
  

  return 0;
  
}
