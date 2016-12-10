#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//Declaration required on linux

union semun {
  int val;
  struct semid_ds *duf;
  unsigned short *array;
  struct seminfo *_buf;
};

int main(){
  int key = ftok("makefile", 20);
  
  int semid = semget( key, 1, 0644);
  
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  
  semop(semid, &sb, 1);

  int f = open("phone.log", O_APPEND | O_RDWR | 0644);
  if (f == -1) {
    printf("Unable to open file!\n");
    exit(1);
  }
  
  int sd = shmget( key, sizeof(int), 0644 );
  int *a = (int *) shmat( sd, 0, 0 );
  
  char last[(*a) + 1];
  lseek(f, -(*a), SEEK_END);
  int i = read(f, last, *a);
  last[i] = '\0';
  if (strlen(last) != 0){
    printf("Previous Message: %s\n", last);
  }
  
  char message[100];
  printf("Enter a message: ");
  fgets(message, sizeof(message), stdin);
  
  *a = strlen(message);
  
  write(f, message, *a);
  close(f);
  
  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
