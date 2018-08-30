#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include "utils.h"


struct car *Cars;

int main (int argc, char* argv[]){
	int i;
	int pidF;
	
	int numsVoit[20] = {44,77,3,33,5,7,31,11,31,19,18,14,2,10,55,8,20,27,30,9};
	// mount the share memory
	int error = mountSHM();
	printf("id |  Race Time | Best Lap \n");
	if(error == -1){
		perror("Error init share memory in race");
		return 1;
	}
	for(i = 0; i < 20; i++){
		shmCar[i].id = numsVoit[i];
		//printf("%d \n",numsVoit[i]);
		shmCar[i].crashed = 0;	
	}
	
	for(i = 0; i<20; i++){
		//printf("%d\n",i);
		pidF = fork();
		if(pidF  == 0) 
		{
			//printf("Car %d \n\tTime for sector 1: %d \n", Cars[i].id, Cars[i].sectorsTime[0]);
		 } 
		else  
		{
			printf("fils");
			char* filePath = "./voiture";
			char buffInt[4];
			sprintf(buffInt, "%d", i);
			char* args[] = {filePath, buffInt, NULL};
			
			if(execv(filePath, args) == -1)
			{
				//printf("%d", i);
				perror("Error exec car");
				return -1;
			}

		}
		
		
	}
	//printf("\n Mtn on demonte la shm \n");
	dismountSHM();
	return 0;

	
}
