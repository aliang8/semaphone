//semaphore control system

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <stat.h>

#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>

//Declaration required on linux
/*
union semun {
	int val;
	struct semid_ds *duf;
	unsigned short *array;
	struct seminfo * _buf;
}
*/


/*
sem_num: the index of the semaphore you want to work on
sem_op: -1 (Down(S)), 1 (Up(S))
*/

int main(int argc, char *argv[]) {

    int semid;
    int key = ftok("makefile" , 22);
    int sc;

    if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    	//create shared mem segment

    	int shmid ;
    	char* data;

    	shmid = shmget( ftok("control.c",12), 1024, IPC_CREAT | 0644 );
    	data = smhat(smhid, (void *)0, 0);

    	//end shared mem
	semid = semget(key, 1, IPC_CREAT | 0644);
	printf("semaphore createdL %d\n", semid);
	union semun su;
	su.val = 1;
	//setting semaphore value
	sc = semctl(semid, 0, SETVAL, su);
	printf("value set: %d\n", sc);
	
    }

    else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
      //opening the file and reading/outputing recursively
      int c;
      FILE *fp;
      fp = fopen("story.txt","r");
      if (fp){
	while ((c = getc(fp)) != EOF)
	  putchar(c);
	fclose(fp);
      }
    }
    
    else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
      semid = semget(key, 1, 0);
      //removing a semaphore
      sc = semctl(semid, 0, IPC_RMID);
      printf("semaphore removed: %d\n", sc);
    }
    return 0;
}
