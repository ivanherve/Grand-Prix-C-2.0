//RANDOM TIME GENERATOR
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

// RANDOM VARs
int randTime;
int randPool[20];

// SHARED MEMORY VARs
key_t keyRtg = 5678;
int shmidRtg;
char *shmaddrRtg;
int sizeRtg = sizeof(int);


// VARIABLES
unsigned int randSeed = 1;

int initSH() {
	if ((shmidRtg = shmget(keyRtg, sizeRtg, IPC_CREAT|0666)) == -1) {
		perror("shmget: shmget failed"); 
		exit(1);
	} else {
		
		//printf("shmid : %d\n",shmidRtg);
		//printf("shmget: shmget succeded !\n");
		return 0;
	}
}

int attachSH(int id) {
	shmaddrRtg = shmat(shmidRtg, NULL, 0);
	if ((shmaddrRtg == (char *)  -1)) {
//printf("shmid : %d\n",shmidRtg);
				
		perror("shmat: shmat failed -------");
//printf("id : %d\n",id);
		exit(1);
	} else {
		//printf("shmat: shmat succeded !\n");
		return 0;
	}
}

int hash (int d){
	int pid = getpid(), result;
	result = d^pid;
	result = result * 891997;
	return abs(result);
}

int randomRtg (int min, int max) {

	// CREATE A SHARED MEMORY
	initSH();	
	//if (initSH() != 0) {
	//	printf("Error by creating a SHM!");	
	//} else {
		attachSH(shmidRtg);
	//}
	

	// CHECK IF SOMETHING IS IN MEMORY
	if (*shmaddrRtg != 0) {
		randSeed = *shmaddrRtg;
	} else {
		//printf("Nothing in Memory\n");
	}

	// GENERATE RANDOM NUMBER
	srand((unsigned)time(NULL) * randSeed );
	
	randTime = rand() % (max-min)+min;
	
	// WRITE NEW RANDSEED ON MEMORY
	*shmaddrRtg = hash(randTime);
	
	return randTime;
}


