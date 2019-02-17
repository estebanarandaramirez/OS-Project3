#include "shop.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#ifndef PI
#define PI 3.14159
#endif

struct thread_args {
  enum customer c;
  double meanArrival;
  double meanCostuming;
  int numVisits;
  int id;
};

struct custStats{
  double costumingTimes[MAX_VISITS];
  double waitTime[MAX_VISITS];
  double goldOwed;
};

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER; // lock for altering shop
double grossRevenue = 0;
double waitTime;

int Customer(struct thread_args *args);
double calc_avg(double last_ave, int count, double new_data);
void mysleep(time_t sleep_time);
void printCustomer(enum customer c);
bool willVisitAgain();
double randomizeTime(double mean);

int main(int argc, char *argv[]) {
  int costuming, pirates, ninjas;
	double costumingTimeP, costumingTimeN, arrivalTimeP, arrivalTimeN;
  srand48(time(0)); // set random seed
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
				if((costumingTimeP < 0) || (costumingTimeP > MAX_TIME)){
					printf("costuming time for pirates has to be between 0 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 5:
				costumingTimeN = atof(argv[i]);
				if((costumingTimeN < 0) || (costumingTimeN > MAX_TIME)){
					printf("costuming time for ninjas has to be between 0 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 6:
				arrivalTimeP = atof(argv[i]);
				if((arrivalTimeP < 0) || (arrivalTimeP > MAX_TIME)){
					printf("Arrival time for pirates has to be between 0 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			case 7:
				arrivalTimeN = atof(argv[i]);
				if((arrivalTimeN < 0) || (arrivalTimeN > MAX_TIME)){
					printf("Arrival time for ninjas has to be between 0 and 1440\n");
					return EXIT_FAILURE;
				}
				break;
			}
		}
	}
  int numUsers = pirates + ninjas;
  struct thread_args *args[numUsers*MAX_VISITS];
  pthread_t threads[numUsers*MAX_VISITS]; // threads struct
  int count = 0;
  Initialize(costuming);
  for(int i = 0; i < pirates; i++){
    args[count] = (struct thread_args *) malloc(sizeof(struct thread_args));
    int visits = 0;
    bool willReturn = true;
    while(willReturn == true && visits <= MAX_VISITS){  // get num of visits for specifified customer
      visits++;
      willReturn = willVisitAgain();
    }
    args[count]->c = pirate;  // input values to thread info struct
    args[count]->meanArrival = arrivalTimeP;
    args[count]->meanCostuming = costumingTimeP;
    args[count]->numVisits = visits;
    args[count]->id = count+1;
    pthread_create(&threads[count], NULL, (void *) &Customer, (void *) args[count]); //create threads
    count++;
  }
  for(int i = 0; i < ninjas; i++){
    args[count] = (struct thread_args *) malloc(sizeof(struct thread_args));
    int visits = 0;
    bool willReturn = true;
    while(willReturn == true && visits <= MAX_VISITS){  // get num of visits for specifified customer
      visits++;
      willReturn = willVisitAgain();
    }
    args[count]->c = ninja; // input values to thread info struct
    args[count]->meanArrival = arrivalTimeN;
    args[count]->meanCostuming = costumingTimeN;
    args[count]->numVisits = visits;
    args[count]->id = count+1;
    pthread_create(&threads[count], NULL, (void *) &Customer, (void *) args[count]); //create threads
    count++;
  }
  for (int i = 0; i < numUsers; i++) {
    pthread_join(threads[i], NULL); // wait for all threads
  }
  Finalize(grossRevenue); // close the shop
  return 0;
}

int Customer(struct thread_args *args){
  time_t time_to_arrive, time_to_costume;
  struct custStats cs;
  int visitCount = args->numVisits;
  for(int i=0; i<visitCount;i++){ // iterate over number of times a thread will visit the shop
    time_to_arrive = (time_t)(randomizeTime(args->meanCostuming)); // gen random number for time to arrive
    mysleep(time_to_arrive); // wait until arrived at the shop
    Enter(args->c); // attempt to enter the shop
    cs.waitTime[i] = waitTime;
    time_to_costume = (time_t)(randomizeTime(args->meanCostuming)); // when in the shop, generate random costuming time
    cs.costumingTimes[i] = time_to_costume;
    if(i == 0){
      cs.goldOwed = 0;
    }
    if(cs.waitTime[i] <= 30){
      cs.goldOwed += time_to_costume;
    }
    mysleep(time_to_costume); // wait for costuming to end
    Leave(); // leave shop
  }
  // print thread info
  pthread_mutex_lock(&print_lock); // Grab printing lock
  printf("Thread Number: %d\n", args->id); // Print thread stats
  printCustomer(args->c);
  printf("Number of Loops: %d\n",visitCount);
  for(int i = 0; i < visitCount; i++){    // Print stats for each visit
    printf("Visit #%d\n", i+1);
    printf("\tWait Time: %lf\n", cs.waitTime[i]);
    printf("\tCostuming Time: %lf\n", cs.costumingTimes[i]);
  }
  printf("Total Gold Owed: %lf\n\n", cs.goldOwed);
  grossRevenue += cs.goldOwed;
  pthread_mutex_unlock(&print_lock); // release printing lock

  pthread_exit(NULL);
  return 0;
}

double calc_avg(double last_ave, int count, double new_data) { // Recalculates an average with a new piece of data
  double new_ave, ratio;
  ratio = (double) count-1;
  ratio = ratio/count;
  new_ave = last_ave*ratio;
  ratio = (double) new_data/count;
  new_ave += ratio;
  return new_ave;
}

void printCustomer(enum customer c){ // Prints the type of customer c
  if (c == pirate){
    printf("Pirate\n");
  }
  else if (c == ninja){
    printf("Ninja\n");
  }
}

void mysleep(time_t sleep_time){ // Sleeps for sleep_time number of microseconds
  if(sleep(sleep_time)!=0){
    printf("Sleep not successful\n");
    exit(-1); // If sleep fails, exit.
  }
}

bool willVisitAgain(){
  bool answer;
  int r = (rand() % (100))+1;
  if (r <= 25) {
    answer = true;
  } else {
    answer = false;
  }
  return answer;
}

double randomizeTime(double upper){
  while(1){
    double a = (double)rand()/(double)(RAND_MAX/1);
    double b = (double)rand()/(double)(RAND_MAX/1);
    double r = sqrt(-2*log(a))*cos(2*M_PI*b);
    double final = r + upper;
    if(final > 0 && final <= MAX_TIME){
      return final;
    }
  }
}

void passWaitTime(double wt){
  waitTime = wt;
}
