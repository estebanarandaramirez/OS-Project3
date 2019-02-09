#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAX_TIME 1440
//make assumption about how many times a pirate/ninja can come back
void singleDaySim(int customing, int pirates, int ninjas, float customingTimeP, float customingTimeN, float arrivalTimeP, float arrivalTimeN);

struct pirate {
  int id;
	float waitTime;
	float customingTime;
	int visits;
  struct command *pirate;
};

struct ninja {
  int id;
	float waitTime;
	float customingTime;
	int visits;
  struct command *ninja;
};

int main(int argc, char *argv[]) {
	int customing, pirates, ninjas;
	float customingTimeP, customingTimeN, arrivalTimeP, arrivalTimeN;
	unsigned int seed = 0;

	if (argc != 8) {		//test to see if the command line has the correct number of arguments
		printf("Usage: ./phase1_project3 #costumingteams #pirates #ninjas customingTimePirates customingTimeNinjas arrivalTimePirates arrivalTimeNinjas\n");
		return EXIT_FAILURE;
	} else {
		for (int i = 1; i < argc; i++){
			switch(i){
			case 1:
				customing = atoi(argv[i]);
        if((customing < 2) || (customing > 4)){
          printf("# of customing teams has to be between 2 and 4\n");
      		return EXIT_FAILURE;
        }
				break;
			case 2:
				pirates = atoi(argv[i]);
        if((pirates < 10) || (pirates > 50)){
          printf("# of pirates has to be between 10 and 50\n");
          return EXIT_FAILURE;
        }
				break;
			case 3:
				ninjas = atoi(argv[i]);
        if((ninjas < 10) || (ninjas > 50)){
          printf("# of ninjas has to be between 10 and 50\n");
          return EXIT_FAILURE;
        }
				break;
			case 4:
				customingTimeP = atof(argv[i]);
				printf("%f\n", customingTimeP);
				if((customingTimeP < 1) || (customingTimeP > MAX_TIME)){
					printf("Customing time for pirates has to be between 1 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 5:
				customingTimeN = atof(argv[i]);
				printf("%f\n", customingTimeN);
				if((customingTimeN < 1) || (customingTimeN > MAX_TIME)){
					printf("Customing time for ninjas has to be between 1 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 6:
				arrivalTimeP = atof(argv[i]);
				printf("%f\n", arrivalTimeP);
				if((arrivalTimeP < 1) || (arrivalTimeP > MAX_TIME)){
					printf("Arrival time for pirates has to be between 1 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 7:
				arrivalTimeN = atof(argv[i]);
				printf("%f\n", arrivalTimeN);
				if((arrivalTimeN < 1) || (arrivalTimeN > MAX_TIME)){
					printf("Arrival time for ninjas has to be between 1 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			}
		}
	}

	if(seed == 0)			//if a seed does equals 0, initialize the random sequence with a random number
		srand(time(NULL));
	else						//otherwise, initialize the random sequence with the value of seed
		srand(seed);

	singleDaySim(customing, pirates, ninjas, customingTimeP, customingTimeN, arrivalTimeP, arrivalTimeN);
	return EXIT_SUCCESS;
}

void singleDaySim(int customing, int pirates, int ninjas, float customingTimeP, float customingTimeN, float arrivalTimeP, float arrivalTimeN){
	int numVisits;
	float busyTimeTeam1 = NULL;
	float freeTimeTeam1 = NULL;
	float busyTimeTeam2 = NULL;
	float freeTimeTeam2 = NULL;
	float busyTimeTeam3 = NULL;
	float freeTimeTeam3 = NULL;
	float busyTimeTeam4 = NULL;
	float freeTimeTeam4 = NULL;
	float averageQueueLength, grossRevenue, goldePerVisit, totalProfits;
	if(busyTimeTeam1 != NULL)
		printf("Time customing team 1 was busy: %f\n", busyTimeTeam1);
	if(freeTimeTeam1 != NULL)
		printf("Time customing team 1 was free: %f\n", freeTimeTeam1);
	if(busyTimeTeam2 != NULL)
		printf("Time customing team 2 was busy: %f\n", busyTimeTeam2);
	if(freeTimeTeam2 != NULL)
		printf("Time customing team 2 was free: %f\n", freeTimeTeam2);
	if(busyTimeTeam3 != NULL)
		printf("Time customing team 3 was busy: %f\n", busyTimeTeam3);
	if(freeTimeTeam3 != NULL)
		printf("Time customing team 3 was free: %f\n", freeTimeTeam3);
	if(busyTimeTeam4 != NULL)
		printf("Time customing team 4 was busy: %f\n", busyTimeTeam4);
	if(freeTimeTeam4 != NULL)
		printf("Time customing team 4 was free: %f\n", freeTimeTeam4);
	printf("Average queue length: %f\n", averageQueueLength);
	printf("Gross revenue: %f\n", grossRevenue);
	printf("Gold per visit: %f\n", goldePerVisit);
	printf("Total profits: %f\n", totalProfits);
}
