#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(){

  srand(time(NULL));

  printf("Forking...\n");
  int f1 = fork();
  int f2;

  if(f1) f2 = fork();
  if(f1 && f2){
    int stat;
    int pid = wait(&stat);

    printf("child pid: %d\t asleep for %d second\n", stat, pid);
    printf("parent process complete\n");
  }

  else{
    printf("child pid:%d\n ", getpid());
    int i = rand() % 4 + 2; // gets a number from 2 to 5
    sleep(i);
    printf("child %d done\n", getpid());
    return i;
  }

  return 0;
}
