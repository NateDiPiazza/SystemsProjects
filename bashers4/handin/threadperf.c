#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <pthread.h>

//this function is called and
//returned after doing no work.
//used to contrast performance from
//processes to threads
void *mythread(){
  //p_thread_exit(NULL); //do no work
  return NULL;
}

int main(int argc, char * argv[]) {

  int i;
  pid_t process;
  if(argc != 3) {
    printf("Usage t <num threads> | p <num processes>\n");
    exit(1);
  }
  if((strcmp(argv[1], "p") != 0) && (strcmp(argv[1], "t") != 0)){
    printf("Usage t <num threads> | p <num processes>\n");
    exit(1);
  }
  int num_iter = atoi(argv[2]);
  if(strcmp(argv[1], "p") == 0){
    for(i = 0; i < num_iter; i++){
  
      process = fork();
      if(process == 0){
        //do no work
	return 0;
      }
      else{
        wait(NULL);
      }
    } //end for num_iter
  } //end if p
  else {
    pthread_t t[num_iter];
    int rc;
    for(i = 0; i < num_iter; i++){
      rc = pthread_create(&t[i], NULL, mythread, NULL);
      pthread_join(t[i], NULL);
    }
    
  }
  
  //p_thread_exit(NULL);
  return 0;
  
}
