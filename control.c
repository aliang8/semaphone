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
#include <sys/shm.h>

//Declaration required on linux

/*
union semun {
  int val;
  struct semid_ds *duf;
  unsigned short *array;
  struct seminfo * _buf;
};
*/

int main(int argc, char *argv[]) {
  int key = ftok("makefile", 22);
  int shm_id, value, semid, fd, sc;
  //struct shmid_ds d;
  
  if(argv[1] == NULL){
    printf("Please provide an argument!\n");
    return 1;
  }
  
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    char* data;

    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semid > 0) {
      //create the text file
      fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
      close(fd);

      value = 1;
      if (argv[2] != NULL){
	value = atoi(argv[2]);
      }

      //creating and attaching shared memory segment
      shm_id = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
      int *shm_ptr = shmat(shm_id,0,0);

      //detaching from shared memory pointer
      shm_ptr = 0;
      shmdt(shm_ptr);

      union semun su;
      su.val = value;
      //setting semaphore value
      sc = semctl(semid, 0, SETVAL, su);
      printf("value set: %d\n", sc);
      
    } else {
      printf("Semaphore already exists!\n");
      return 1;
    } 
    
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
    int smc = shmctl(semid, IPC_RMID, 0); //&sm
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
