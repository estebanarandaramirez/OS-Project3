#include <stdbool.h>
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

#define MAX_TIME 1440 //number of seconds to simulate a day
#define MAX_VISITS 20 //max number of visits possible

struct costumingTeam {
  int id;
  bool isFree;
  float busyTime;
  float freeTime;
};

struct customer {
  int flag;   //0 for pirates, 1 for ninjas
  int id;
  int visits;
  pthread_t thread;
  int costumingTeam[MAX_VISITS];
	float waitTime[MAX_VISITS];
	float costumingTime[MAX_VISITS];
  struct customer *next;
};

struct customer* initializeCustomers(int costuming, int pirates, int ninjas, float costumingTimeP, float costumingTimeN, float arrivalTimeP, float arrivalTimeN);
float randomizeArrivalTime(float upper);
float randomizeCostumingTime(float upper);
void singleDaySim(int costuming, int pirates, int ninjas, float costumingTimeP, float costumingTimeN, float arrivalTimeP, float arrivalTimeN, struct customer *customerHead, struct costumingTeam *arrayTeams);

int main(int argc, char *argv[]) {
	int costuming, pirates, ninjas;
	float costumingTimeP, costumingTimeN, arrivalTimeP, arrivalTimeN;
	unsigned int seed = 0;

	if (argc != 8) {		//test to see if the command line has the correct number of arguments
		printf("Usage: ./phase1_project3 #costumingteams #pirates #ninjas costumingTimePirates costumingTimeNinjas arrivalTimePirates arrivalTimeNinjas\n");
		return EXIT_FAILURE;
	} else {
		for (int i = 1; i < argc; i++){
			switch(i){
			case 1:
				costuming = atoi(argv[i]);
        if((costuming < 2) || (costuming > 4)){
          printf("# of costuming teams has to be between 2 and 4\n");
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
				costumingTimeP = atof(argv[i]);
				if((costumingTimeP < 3) || (costumingTimeP > MAX_TIME)){
					printf("costuming time for pirates has to be between 3 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 5:
				costumingTimeN = atof(argv[i]);
				if((costumingTimeN < 3) || (costumingTimeN > MAX_TIME)){
					printf("costuming time for ninjas has to be between 3 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 6:
				arrivalTimeP = atof(argv[i]);
				if((arrivalTimeP < 3) || (arrivalTimeP > MAX_TIME)){
					printf("Arrival time for pirates has to be between 3 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 7:
				arrivalTimeN = atof(argv[i]);
				if((arrivalTimeN < 3) || (arrivalTimeN > MAX_TIME)){
					printf("Arrival time for ninjas has to be between 3 and 1440\n");
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

  struct costumingTeam* arrayTeams = malloc(costuming * sizeof(*arrayTeams));
  for(int i = 1; i <= costuming; i++){
    arrayTeams[i].id = i;
    arrayTeams[i].isFree = true;
  }

  struct customer *customerHead;
  customerHead = (struct customer*)malloc(sizeof(struct customer));
  customerHead = initializeCustomers(costuming, pirates, ninjas, costumingTimeP, costumingTimeN, arrivalTimeP, arrivalTimeN);

	singleDaySim(costuming, pirates, ninjas, costumingTimeP, costumingTimeN, arrivalTimeP, arrivalTimeN, customerHead, arrayTeams);
	return EXIT_SUCCESS;
}

struct customer* initializeCustomers(int costuming, int pirates, int ninjas, float costumingTimeP, float costumingTimeN, float arrivalTimeP, float arrivalTimeN){
  int counter = 1;

  struct customer *customerHead;
  customerHead = (struct customer*)malloc(sizeof(struct customer));
  customerHead->flag = 0;
  customerHead->id = counter;
  customerHead->visits = 1;
  customerHead->costumingTeam[0] = 1;
  customerHead->waitTime[0] = 0;
  customerHead->costumingTime[0] = costumingTimeP;
  customerHead->next = NULL;
  counter++;

  struct customer *temp1;
  temp1 = (struct customer*)malloc(sizeof(struct customer));
  temp1 = customerHead;
  for(int i = 0; i < pirates; i++){
    struct customer *temp2;
    temp2 = (struct customer*)malloc(sizeof(struct customer));
    temp2->flag = 0;
    temp2->id = counter;
    temp2->visits = 1;
    temp2->costumingTeam[0] = 1;
    temp2->waitTime[0] = 0;
    temp2->costumingTime[0] = costumingTimeP;
    temp2->next = NULL;
    counter++;
    temp1->next = temp2;
    temp1 = temp2;
  }
  for(int i = 0; i < ninjas; i++){
    struct customer *temp2;
    temp2 = (struct customer*)malloc(sizeof(struct customer));
    temp2->flag = 1;
    temp2->id = counter;
    temp2->visits = 1;
    temp2->costumingTeam[0] = 1;
    temp2->waitTime[0] = 0;
    temp2->costumingTime[0] = costumingTimeN;
    temp2->next = NULL;
    counter++;
    temp1->next = temp2;
    temp1 = temp2;
  }
  return customerHead;
}

float randomizeArrivalTime(float upper){
  float a = (float)rand()/(float)(RAND_MAX/1);
  float b = (float)rand()/(float)(RAND_MAX/1);
  float r = sqrt(-2*log(a))*cos(2*M_PI*b);
  float average = r + upper;
  float randomTimeOfDay = (float)rand()/(float)(RAND_MAX/MAX_TIME);
  float final = randomTimeOfDay + average;
  return final;
}

float randomizeCostumingTime(float upper){
  float a = (float)rand()/(float)(RAND_MAX/1);
  float b = (float)rand()/(float)(RAND_MAX/1);
  float r = sqrt(-2*log(a))*cos(2*M_PI*b);
  float final = r + upper;
  return final;
}

void singleDaySim(int costuming, int pirates, int ninjas, float costumingTimeP, float costumingTimeN, float arrivalTimeP, float arrivalTimeN, struct customer *customerHead, struct costumingTeam *arrayTeams){
	int numVisits = 0;
	float totalQueueLength = 0, averageQueueLength, grossRevenue = 0, goldePerVisit, totalProfits;


  //Print statistics
  for(int j = 0; j < (pirates+ninjas); j++){
    float goldOwed = 0;
    if(customerHead->flag == 0){
      printf("Pirate #%d:\n", customerHead->id);
    } else if(customerHead->flag == 1){
      printf("Ninja #%d:\n", customerHead->id);
    }
    printf("Number of visits: %d\n", customerHead->visits);
    for(int i = 0; i < customerHead->visits; i++){
      printf("Visit #%d:\n", i+1);
      printf("\tCostuming time: %f\n", customerHead->costumingTime[i]);
      printf("\tWait time: %f\n", customerHead->waitTime[i]);
      totalQueueLength += customerHead->waitTime[i];
      if(customerHead->waitTime[i] <= 30){
        goldOwed += customerHead->costumingTime[i];
      }
      arrayTeams[customerHead->costumingTeam[i]].busyTime +=  customerHead->costumingTime[i];
    }
    printf("Total gold owed: %f\n\n", goldOwed);
    numVisits += customerHead->visits;
    grossRevenue += goldOwed;
    if(customerHead->next != NULL){
      customerHead = customerHead->next;
    } else {
      printf("Out of bounds\n");
      return EXIT_FAILURE;
    }
  }

  averageQueueLength = (totalQueueLength)/(numVisits);
  goldePerVisit = (grossRevenue)/(numVisits);
  totalProfits = (grossRevenue)-(5*costuming);

  for(int i = 1; i <= costuming; i++){
    printf("Time costuming team %d was busy: %f\n", i, arrayTeams[i].busyTime);
    printf("Time costuming team %d was free: %f\n", i, arrayTeams[i].freeTime);
  }
	printf("Average queue length: %f\n", averageQueueLength);
	printf("Gross revenue: %f\n", grossRevenue);
	printf("Gold per visit: %f\n", goldePerVisit);
	printf("Total profits: %f\n", totalProfits);
}
