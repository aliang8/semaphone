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

int main(){
  int key = ftok("makefile", 22);
  int semid = semget(key, 1, 0644);
  int fd;
  int shm_id;
  int *shm_ptr;

  //sembuf
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  
  semop(semid, &sb, 1);

  fd = open("story.txt", O_APPEND | O_RDWR | 0644);
  if (!fd) {
    printf("File does not exist!\n");
    exit(1);
  }
  
  //accessing shared memory segment
  shm_id = shmget(key, sizeof(int), 0644 );
  shm_ptr = (int *) shmat(shm_id, 0, 0 );
  
  char last[(*shm_ptr) + 1];
  lseek(fd, -(*shm_ptr), SEEK_END);
  int i = read(fd, last, *shm_ptr);
  last[i] = '\0';
  if (strlen(last) != 0){
    printf("Previous Message: %s\n", last);
  }
  
  char message[100];
  printf("Enter a message: ");
  fgets(message, sizeof(message), stdin);
  
  *shm_ptr = strlen(message);
  
  write(fd, message, *shm_ptr);
  close(fd);
  
  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
