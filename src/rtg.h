//RANDOM TIME GENERATOR
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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


int randomRtg (int min, int max) {

	// CREATE A SHARED MEMORY
	initSH();	
	attachSH(shmid);
	

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

int initSH() {
	if ((shmidRtg = shmget(keyRtg, sizeRtg, IPC_CREAT|0660)) == -1) {
		perror("shmget: shmget failed"); 
		exit(1);
	} else {
		//printf("shmget: shmget succeded !\n");
		return 0;
	}
}

int attachSH(int id) {
	if ((shmaddrRtg = shmat(id, NULL, 0)) == (char *)  -1) {
		perror("shmat: shmat failed");
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
