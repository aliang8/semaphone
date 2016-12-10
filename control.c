//semaphore control system

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
  struct seminfo * _buf;
};

  
/*
  sem_num: the index of the semaphore you want to work on
  sem_op: -1 (Down(S)), 1 (Up(S))
*/

int main(int argc, char *argv[]) {
  
  int semid;
  int key = ftok("makefile" , 22);
  int sc; 
  struct shmid_ds d;
  
  if(argv[1] == NULL){
    printf("Please provide an argument!\n");
    return 1;
  }
  
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    //create shared mem segment    
    int shm_id;
    char* data;
    int fd;
    int *shm_ptr;
    
    shm_id = shmget( ftok("story.txt",22), 1024, IPC_CREAT | 0644 );
    if (shm_id < 0){
      printf("shmget error\n");
      exit(1);
    }
    
    //attaching the shared memory segment
    shm_ptr = (int *)shmat(shm_id, NULL, 0);
    if ((int) shm_ptr == -1){
      printf("shmat error\n");
      exit(1);
    }
    
    //created a semaphore
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semid) {
      //create the text file
      fd = open("story.txt", O_CREAT | 0644);
      close(fd);
    }
    
        
    union semun su;
    su.val = 1;
    //setting semaphore value
    sc = semctl(semid, 0, SETVAL, su);
    printf("value set: %d\n", sc);
    
  } else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    //opening the file and reading/outputing recursively
    int c;
    FILE *fp;
    fp = fopen("story.txt","r");
    if (fp){
      while ((c = getc(fp)) != EOF)
	putchar(c);
      fclose(fp);
    }
    return 0;
  } else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    //removing a semaphore and shared memory segment
    int sm = shmget(key,4,0644);
    semid = semget(key, 1, 0);
    
    //removing semaphore
    sc = semctl(semid, 0, IPC_RMID);
    //removing sm segment
    int smc = shmctl(semid, IPC_RMID, &sm);
    printf("Story removed: %d\n", sc);
    
    //print out the story one more time
    int c;
    FILE *fp;
    fp = fopen("story.txt","r");
    if (fp){
      while ((c = getc(fp)) != EOF)
	putchar(c);
      fclose(fp);
    }
    return 0;
  }
  return 0;
}
