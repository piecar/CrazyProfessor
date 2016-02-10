#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ITEMS 100

// shared buffer
long buf[MAX_ITEMS];

// number of items in the buffer
int count = 0;

// shared lock
pthread_mutex_t lock;

// shared conditions
pthread_cond_t notfull;
pthread_cond_t notempty;

int producer_index = 0;
int consumer_index = 0 ;

void init() {

  pthread_mutex_init(&lock, NULL);

  pthread_cond_init(&notfull, NULL);
  pthread_cond_init(&notempty, NULL);

}


void * Producer(void * p) {


  printf("Created producer thread ....\n");

  while(1) {
    
    // ENTRY SECTION
    // TODO: check count first
    // before producing
    // wait if count == MAX_ITEMS
    // do we need an empty slot?
    pthread_mutex_lock(&lock);
    while(count==MAX_ITEMS) {
      pthread_cond_wait(&notfull, &lock);
    }

    // CRITICAL SECTION
    // produce the next item
    int produced = random()%100;
    buf[producer_index] = produced;
    producer_index = (producer_index + 1)%MAX_ITEMS;
    count++;

    printf("Produced[%d] %d\n", producer_index, produced);
    
    // EXIT SECTION
    // unlock mutex
    // signal consumer
    pthread_cond_signal(&notempty);
    pthread_mutex_unlock(&lock);

  }
  

  return NULL;

  // if you want to pass something
  // to a joining thread call:
  // pthread_exit(xyz);

}


void * Consumer(void * p) {

  
  printf("Created consumer thread ....\n");
  
  while(1) {

    // ENTRY SECTION
    // check if buffer is empty

    if(consumer_index%10==0) {
      sleep(1);
    }
    
    pthread_mutex_lock(&lock);
    while(count==0) {
      pthread_cond_wait(&notempty, &lock);
    }
    
    // CRITICAL SECTION
    // consume the next item
    int consumed = buf[consumer_index];
    count--;
    consumer_index = (consumer_index + 1) % MAX_ITEMS;

    printf("Consumed[%d] %d\n", consumer_index, consumed);
    
    // EXIT SECTION
    pthread_cond_signal(&notfull);
    pthread_mutex_unlock(&lock);
    
  }

  return NULL;

}

int main (int argc, char *argv []) {

  pthread_t producer, consumer;

  // initialize mutex and
  // condition variables
  init();
  
  if(pthread_create(&producer, NULL, Producer, NULL)!=0) {
    perror("Producer thread could not be created!\n");
    exit(0);
  }

  if(pthread_create(&consumer, NULL, Consumer, NULL)!=0) {
    perror("Consumer thread could not be created!\n");
    exit(0);
  }

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);
  

  return 0;
  
}
