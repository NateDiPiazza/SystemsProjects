#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

unsigned int seed;
int max;
int num_trans;
int balance;
pthread_mutex_t lock;

//void* transaction(void* bal){
void* transaction(void* bal){
  
  //int balance;
  //balance = (int) bal;
//  printf("Balance in trans %d\n", *balance);
  int i, r, val, rc;
  //*balance = 0;
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
        //printf("Random %d\n", val);
      }
    pthread_mutex_unlock(&lock);
  }
  //printf("Balance in trans %d\n", balance);
  //return current balance
  return (void*) rc;
  //return NULL;
}

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
  
  //TODO start seperate thread
  
  // for each: generate rand num with random_r
  // call srandom in each thread
  
  //check if val < balance
  // if yes, lock-transaction-release lock
  
  
  
  balance = init_bal; //set the balance to be the initial amount given
  int i, j, rc;
  rc = pthread_mutex_init(&lock, NULL);
  assert(rc == 0);
  pthread_t threads[num_threads];
  for(i = 0; i < num_threads; i++){
    //pthread_create(&threads[i], NULL, transaction, (void*)balance);
    pthread_create(&threads[i], NULL, transaction, NULL);
  }
  for(j=0; j < num_threads; j++){
    pthread_join(threads[j], (void**) &rc);
    if(rc == 1){
      //fprintf(stderr, "Attempted to withdrawl more money than in account.\n");
    }
    //pthread_join(threads[j], NULL);
  }
  //call exit
  printf("Final bank balance: %d\n", balance);
  pthread_mutex_destroy(&lock);
  pthread_exit(NULL);
  
  return 0;
}
