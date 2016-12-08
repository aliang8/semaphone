//semaphore control system

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>

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
    	/*
    	if( semid >= 0 ) {
    		printf("semaphore created: %d\n", semid);

    		int value = atoi(s);
    		union semun su;
    		su.val = value;

    		sc = semctl( semid, 0, SETVAL, su );
    		printf("value set: %d\n", sc );
    	}
    	else {
    		printf("exists");
    	}
    	*/

		semid = semget(key, 1, IPC_CREAT | 0644);
		printf("semaphore createdL %d\n", semid);
		union semun su;
		su.val = 1;
		//setting semaphore value
		sc = semctl(semid, 0, SETVAL, su);
		printf("value set: %d\n", sc);

		}

		else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
		semid = semget(key, 1, 0);
		//getting the value of a semaphore
		sc = semctl(semid, 0, GETVAL);

		printf("semaphore value: %d\n",sc);
		}

		else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
		semid = semget(key, 1, 0);
		//removing a semaphore
		sc = semctl(semid, 0, IPC_RMID);
		printf("semaphore removed: %d\n", sc);
		}


		return 0;

}