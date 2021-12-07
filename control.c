#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// header for semaphores
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SEMKEY 24602
#define SHMKEY 24603 

int main(int argc, char * argv[]){
  int err, shmd, semd;
  if(argv[1] == NULL) printf("no argument detected\n");


  // open the shared memory, semaphore and file
  else if(strcmp(argv[1], "-create") == 0){
    // shmget: creates the segment once
    shmd = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0644);
    // semget: create/get access to semaphore
    // semget( key, amount, flags)
    semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if(semd == -1) printf("error: %s", strerror(errno));

    // set value of semaphore to 1 
    union semun us;
    us.val = 1;
    err = semctl(semd, 0, SETVAL, us);
    if(err == -1) printf("error: %s", strerror(errno));

    err = open("phone.txt", O_CREAT | O_TRUNC, 0644);
    if(err == -1) {
      printf("error: %s", strerror(errno));
      return 0;
    }
  }

  else if (strcmp(argv[1], "-remove") == 0){
    semd = semget(SEMKEY, 1, 0);
    int shmd = shmget(SHMKEY, 1, 0);
    shmctl(shmd, IPC_RMID, 0);
    semctl(semd, IPC_RMID, 0);

    int file = open("phone.txt", O_RDONLY);
    if(file == -1){ 
      printf("error: %s", strerror(errno));
      return 0;
    }
    struct stat st;
    stat("phone.txt", &st);
    char *buff = malloc(st.st_size);
    err = read(file, buff, st.st_size);
    if(err == -1){
      printf("error: %s", strerror(errno));
      return 0;
    }
    printf("Story so far:\n");
    printf("%s\n", buff);
    free(buff);
  }
  return 0;
}