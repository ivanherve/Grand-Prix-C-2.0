struct car {
         int id;
         int pitFlag;
         int  crashed;
         int sectorsTime[3];
         int lapTime;
         int pitTime;
         int raceTime;
	 int lap;
};

// SHARED MEMORY

key_t key = 6789;
int shmid;
struct car *shmCar;
int size = sizeof(struct car) * 20;
struct car *carat;  

int mountSHM() {
        // Init Shared Memory
        if ((shmid = shmget(key, size, IPC_CREAT|0666)) == -1) {
                perror("shmget: shmget failed");
                return -1;
        } else {
                // Attach SHM
                if ((shmCar = shmat(shmid, NULL, 0)) == (struct car*)  -1) {
                       	perror("shmat: shmat failed ----- ");
                        return -1;
                } else {
                        return 0;
                }
        }
	return 0;
}

int attachSHM() {
	 if ((shmid = shmget(key, size, 0666)) == -1) {
                perror("shmget: shmget failed");
                return -1;
        } else {
                // Attach SHM
                if ((shmCar = shmat(shmid, NULL, 0)) == (struct car*)  -1) {
                       	perror("shmat: shmat failed ----- ");
                        return -1;
                } else {
                        return 0;
                }
        }
	return 0;

}

int dismountSHM(){
	shmdt(shmCar);
	shmctl(shmid, IPC_RMID, NULL); 
}
