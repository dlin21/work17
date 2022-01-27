#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

// for semaphores
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>

#define SEMKEY 24602
#define SHMKEY 24603

int main(){
  int semd = semget(SEMKEY,1,0);
  int shmd = shmget(SHMKEY,1,0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  
  int *len = shmat(shmd,0,0);
  int f = open("phone.txt",O_RDONLY);
  lseek(f,-1 * *len,SEEK_END);
  char line[4096];
  int err = read(f,line,*len);
  if(err == -1) {
      printf("error: %s\n", strerror(errno));
      return 0;
  }
  line[*len]='\0';
  printf("last line: %s\n",line);

  printf("new line: ");
  fgets(line,4096,stdin);
  *len = strlen(line);

  // Write line to phone.txt
  f = open("phone.txt", O_WRONLY | O_APPEND);
  if(f == -1) {
      printf("error: %s\n", strerror(errno));
      return 0;
  }
  
  err = write(f,line,strlen(line));
  if(err == -1) {
      printf("error: %s\n", strerror(errno));
      return 0;
  }
  shmdt(len);
  sb.sem_op = 1; 
  semop(semd, &sb, 1);

  return 0;
}
