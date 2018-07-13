#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "utils.h"




int main (int argc, char* argv[]){
	int i;
	int pidF;
	int numsVoit[20] = {44,77,3,33,5,7,31,11,31,19,18,14,2,10,55,8,20,27,30,9};
	// mount the share memory
	if(mountSHM() == -1){
		perror("Error init share memory in race");
		return -1;
	} else {
		printf("Shared Memory mounted !\n");
	}
	//init the sharmemory
	for(i = 0; i < 20; i++){
		shmCar[i].id = numsVoit[i];
		shmCar[i].crashed = 0;
	}

	//creat the carsi
	i = 0;
	for(i = 0; i < 20; i++){
		if((pidF = fork()) == -1){
			perror("Error fork");
			return -1;
		}
		if(pidF == 0){

			printf("Car %d: time for sector 1 : %d \n", shmCar[i].id, shmCar[i].sectorsTime[0]);

		} else {
		
			char* filePath = "voiture";
			char buffInt[4];
			sprintf(buffInt, "%d", i);
			char* args[] = {filePath, buffInt, NULL};
			
			if(execv(filePath, args) == -1){
				printf(" => %d, ", i);
				perror(" -> Error exec car");
				return -1;
			}

		}
		
	}


	return 0;
	
} 
