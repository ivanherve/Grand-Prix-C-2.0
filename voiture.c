#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "utils.h"
#include "rtg.h"

int memPos;
struct car Car;
int raceIsOver = 0;
int lap = 0;


int checkBestSectorTime(int sector) {
	// Check if the new sector time is better than the last best one (general)
	return shmCar[memPos].sectorsTime[sector] < Car.sectorsTime[sector];
}

void writeSectorTime(int sector) {
	// Write the best sector time in the Shared Memory
	if (checkBestSectorTime(sector)) {
		shmCar[memPos].sectorsTime[sector] = Car.sectorsTime[sector];
	}
}



int calcLap() {
	// Calc the total lap time
	int totalLap = 0, i;
	for (i = 0; i < sizeof(Car.sectorsTime)/sizeof(int); i++) {
		totalLap += Car.sectorsTime[i];
	}
	return totalLap;
	
}

void writeLapTime() {
	// Write the lap time in the Shared Memory
	if (shmCar[memPos].lapTime < Car.lapTime) {
		shmCar[memPos].lapTime = Car.lapTime;
	}
}

int getTime(int min, int max) {
	// Call the time generator function
	return randomRtg(min, max);
	
}


int isCrashed() {
	// decide randomly if car is crashed
	// 5% chance of crash
	//srand(time(NULL));
	
	return 0;	
}
	
int isPit() {
	// decide randomly is car go to pit
	// 20% change of pit stop
	return !Car.pitFlag;
}


int main (int argc, char *argv[]) {
	
	//printf("Bonjour je suis la voiture %s \n", argv[1]);
	
	// Create and mount the shared memory
	if (attachSHM() == -1){
		printf("Creation or Mounting of SHM failed car \n");
		return 1;		
	}
	
	// Find position in SHM
	if(argc < 2){
		printf("argv no complete");
		return -1 ;
	}
	memPos = atoi(argv[1]);
	

	// Init Car	
	Car = shmCar[memPos];
	Car.raceTime = 0;

	// Race simulation
	int j = 0;
	while (!raceIsOver && !Car.crashed) { // Loop while !raceisOver
		int i;
		for (i = 0; i < sizeof(Car.sectorsTime)/sizeof(int) && !Car.crashed; i++) {		
			
			//printf("Loop \n");			

			if (!Car.crashed) { // isCrashed ?
				Car.sectorsTime[i] = getTime(25, 35); // Generate sector time
				//printf("Time %d generated for car n°: %d\n",Car.sectorsTime[i], Car.id);
				writeSectorTime(i);
			}
			
		}

		if (isPit() && !Car.crashed) {
			Car.pitFlag = 1;
			Car.pitTime = getTime(5, 10);
		}

		if (!Car.crashed) {
			Car.lapTime = calcLap();
			lap ++;
			Car.raceTime += Car.lapTime;
			if (lap > 25){
				raceIsOver = 1;
			}
		}
	
	}
	sleep(2);
	printf("%d  | %d  | %d \n",Car.id,Car.raceTime,Car.lapTime); 
	return 0;	
}


