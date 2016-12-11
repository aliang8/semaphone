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
#include <sys/shm.h>

int main(){
  int key = ftok("makefile", 22);
  int semid = semget(key, 1, IPC_CREAT | 0644);
  int fw, fr, shm_id;

  //sembuf
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  
  //requesting the semaphore
  semop(semid, &sb, 1);

  fw = open("story.txt", O_CREAT | O_APPEND | O_WRONLY, 0644);
  fr = open("story.txt", O_RDONLY);
  if (!fr) {
    printf("File does not exist!\n");
    exit(1);
  }
  
  //accessing shared memory segment
  shm_id = shmget(key, sizeof(int), IPC_CREAT | 0644 );
  int *shm_ptr = shmat(shm_id, 0, 0);
  
  //outputting the previous line 
  int len = *(shm_ptr);
  char prev[len + 1];
  lseek(fr, -1 * len, SEEK_END);
  read(fr, prev, len);
  prev[len] = 0;
  close(fr);
  if(len >= 2){
    printf("Previous addition: %s\n", prev);
  }

  char message[1024];
  printf("Continue the story: \n");
  fgets(message, sizeof(message), stdin);
  
  //writing to the file
  write(fw, message, strlen(message));
  close(fw);
  
  //updating the last length
  *shm_ptr = strlen(message);
  
  //releasing the value from semaphore
  sb.sem_num = 1;
  semop(semid, &sb, 1);
  
  return 0;
}
