#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ITEMS 100
#define PRODUCERS 5
#define CONSUMERS 10

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

  int pid = (int)p;

  printf("Created Producer[%d] ....\n", pid);

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

    printf("Producer[%d] produced[%d] %d\n", pid, producer_index, produced);
    
    // EXIT SECTION
    // unlock mutex
    // signal consumer
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&notempty);

  }
  

  return NULL;

  // if you want to pass something
  // to a joining thread call:
  // pthread_exit(xyz);

}


void * Consumer(void * c) {

  int cid = (int)c;
  
  printf("Created consumer thread ....\n");
  
  while(1) {

    // ENTRY SECTION
    // check if buffer is empty


    // How do we synchronize
    // access to consumer_index?
    // Homework!
    // You should not be sleeping with
    // lock held -- a lot of other
    // threads may get needlessly blocked!
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

    printf("Consumer[%d] consumed[%d] %d\n", cid, consumer_index, consumed);
    
    // EXIT SECTION
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&notfull);
    
  }

  return NULL;

}

int main (int argc, char *argv []) {

  pthread_t producer[PRODUCERS], consumer[CONSUMERS];

  // initialize mutex and
  // condition variables
  init();
  long int i;
  
  for(i=0; i < PRODUCERS; i++) {
    if(pthread_create(&producer[i], NULL, Producer, (void *)i)!=0) {
      perror("Producer could not be created!\n");
      exit(0);
    }
  }

  for(i=0; i < CONSUMERS; i++) {
    if(pthread_create(&consumer[i], NULL, Consumer, (void *)i)!=0) {
      perror("Consumer thread could not be created!\n");
      exit(0);
    }
  }

  for(i=0; i < PRODUCERS; i++) {
    pthread_join(producer[i], NULL);
  }

  for(i=0; i < CONSUMERS; i++) {
    pthread_join(consumer[i], NULL);
  }
  
  return 0;
  
}
