#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

unsigned int seed;
int max;
int num_trans;
int balance;
pthread_mutex_t lock;

/**
 * This is a multithreaded function
 * that performs banking operations.
 * When the shared variable balance 
 * is modified it is locked to prevent
 * a race condition.
**/
void* transaction(void* bal){
  
  int i, r, val, rc;
  char statebuf[32] = {0};
  struct random_data rd = {0};
  rc = 0; //initialize to good
  initstate_r(seed, statebuf, sizeof(statebuf), &rd);
  srandom_r(seed, &rd);
  for(i = 0; i < num_trans; i++){
    pthread_mutex_lock(&lock);
      random_r(&rd, &r);
      val = r % (2*max+1)-max; //thank you Prof. Swift
      if((val < 0) && ((val*-1) > balance)){
        //cannot go in the hole e.g. no overdrafts allowed
        rc = 1;
      }
      else {
        balance += val;
      }
    pthread_mutex_unlock(&lock);
  }
  return (void*) rc;
}

/**
 * Main: The main program creates
 * a set number of threads and carries
 * out a series of banking transactions.
 *
 * @author Nate DiPiazza
 **/
int main(int argc, char * argv[]) {

  if(argc != 6){
    fprintf(stderr, "Usage: bank <number of threads> <number of transactions> <max transaction size> <initial balance> <seed>\n");
    return 1;
  }
  //Assign params
  int const num_threads = atoi(argv[1]);
  num_trans = atoi(argv[2]);
  int const init_bal = (int) strtol(argv[4],NULL,10);
  max = (int) strtol(argv[3],NULL,10);
  seed = (unsigned int) strtol(argv[5], NULL, 10);
  
  balance = init_bal; //set the balance to be the initial amount given
  int i, j, rc;
  rc = pthread_mutex_init(&lock, NULL); //initialize mutex lock needed for transaction()
  assert(rc == 0); //validate lock intialization
  //create the threads and let them get started
  pthread_t threads[num_threads];
  for(i = 0; i < num_threads; i++){
    pthread_create(&threads[i], NULL, transaction, NULL);
  }
  for(j=0; j < num_threads; j++){
    pthread_join(threads[j], (void**) &rc);
    if(rc == 1){
    }
  }
  //call exit
  printf("Final bank balance: %d\n", balance);
  pthread_mutex_destroy(&lock);
  pthread_exit(NULL);
  
  return 0;
}
