#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CARS 20
sem_t nw;
sem_t ne;
sem_t sw;
sem_t se;
int globalCount = 0;

typedef enum{
	N,S,E,W
} approach;

typedef enum{
	Straight,Right,Left
} turnType;

typedef struct car{
  int id;
  approach approach;
  turnType turnType;
  struct car *next;
}car;

typedef struct intersectionQueue{
	struct car *next;
}intersectionQueue;

void handleThreads(car *car_info);
void printDirections(approach a);
void printTurn(turnType t);
int whoPasses();
void *Intersection(void* arg){


}

void simulateIntersection(){
  sem_init(&nw, 0, 1);
  sem_init(&ne, 0, 1);
  sem_init(&sw, 0, 1);
  sem_init(&se, 0, 1);
  pthread_t car1_t, car2_t, car3_t, car4_t;
	int nCount=0;
	int sCount=0;
	int eCount=0;
	int wCount=0;
	srand(time(NULL));
	for(int i=0; i<MAX_CARS; i++){
		switch(rand()%4){
			case 0:
			printf("North added\n");
			nCount++;
			break;
			case 1:
			printf("South added\n");
			sCount++;
			break;
			case 2:
			printf("East added\n");
			eCount++;
			break;
			case 3:
			printf("West added\n");
			wCount++;
			break;
			default:
			printf("Something went horribly wrong");
			break;
		}}
		car *nHead=NULL;
		nHead = malloc(sizeof(car));
    	nHead->approach=N;
    	nHead->turnType=rand()%3;
    	nHead->next = NULL;
    	printf("North Head: %d Approach: %d Turn Type: %d\n",nHead->id, nHead->approach, nHead->turnType);
    	car *temp1;
    	temp1=malloc(sizeof(car));
    	temp1=nHead;
    	for(int i=1; i<=nCount-1; i++){
			car *temp2;
			temp2=malloc(sizeof(car));
			temp2->id=i;
			temp2->approach=N;
			temp2->turnType=rand()%3;
			temp2->next=NULL;
			temp1->next=temp2;
			temp1 = temp2;
			printf("North Id: %d Approach: %d Turn Type: %d\n",temp2->id, temp2->approach, temp2->turnType);
    }

		car *sHead=NULL;
		sHead = malloc(sizeof(car));
		sHead->approach=S;
		sHead->turnType=rand()%3;
		sHead->next = NULL;
		printf("South Head: %d Approach: %d Turn Type: %d\n",sHead->id, sHead->approach, sHead->turnType);
		temp1=sHead;
		for(int i=1; i<=sCount-1; i++){
			car *temp2;
			temp2=malloc(sizeof(car));
			temp2->id=i;
			temp2->approach=S;
			temp2->turnType=rand()%3;
			temp2->next=NULL;
			temp1->next=temp2;
			temp1 = temp2;
			printf("South Id: %d Approach: %d Turn Type: %d\n",temp2->id, temp2->approach, temp2->turnType);
		}

		car *eHead=NULL;
		eHead = malloc(sizeof(car));
		eHead->approach=E;
		eHead->turnType=rand()%3;
		eHead->next = NULL;
		printf("East Head: %d Approach: %d Turn Type: %d\n",eHead->id, eHead->approach, eHead->turnType);
		temp1=eHead;
		for(int i=1; i<=eCount-1; i++){
			car *temp2;
			temp2=malloc(sizeof(car));
			temp2->id=i;
			temp2->approach=E;
			temp2->turnType=rand()%3;
			temp2->next=NULL;
			temp1->next=temp2;
			temp1 = temp2;
			printf("East  Id: %d Approach: %d Turn Type: %d\n",temp2->id, temp2->approach, temp2->turnType);
		}

		car *wHead=NULL;
		wHead = malloc(sizeof(car));
		wHead->approach=W;
		wHead->turnType=rand()%3;
		wHead->next = NULL;
		printf("West Head: %d Approach: %d Turn Type: %d\n",wHead->id, wHead->approach, wHead->turnType);
		temp1=wHead;
		for(int i=1; i<=wCount-1; i++){
			car *temp2;
			temp2=malloc(sizeof(car));
			temp2->id=i;
			temp2->approach=W;
			temp2->turnType=rand()%3;
			temp2->next=NULL;
			temp1->next=temp2;
			temp1 = temp2;
			printf("West  Id: %d Approach: %d Turn Type: %d\n",temp2->id, temp2->approach, temp2->turnType);
		}

		// car *nHead;
		// car *sHead;
		// car *eHead;
		// car *wHead;
		// intersectionQueue *inter;
    //
		// nHead = malloc(sizeof(car));
		// sHead = malloc(sizeof(car));
		// eHead = malloc(sizeof(car));
		// wHead = malloc(sizeof(car));
		// inter = malloc(sizeof(intersectionQueue));

		// sem_init(&nw, 0, 1);
		// sem_init(&ne, 0, 1);
		// sem_init(&sw, 0, 1);
		// sem_init(&se, 0, 1);
		// pthread_t car1_t, car2_t, car3_t, car4_t;
		// pthread_create(&car1_t, NULL, nHandle, NULL);
		// pthread_create(&car2_t, NULL, sHandle, NULL);
		// pthread_create(&car3_t, NULL, eHandle, NULL);
		// pthread_create(&car4_t, NULL, wHandle, NULL);

		// nHead=nHead;
		// sHead=sHead;
		// eHead=eHead;
		// wHead=wHead;

		int nCost=3;
		int sCost=3;
		int eCost=3;
		int wCost=3;

		printf("North Intersection Id: %d Approach: %d Turn Type: %d\n",nHead->id, nHead->approach, nHead->turnType);
		printf("South Intersection Id: %d Approach: %d Turn Type: %d\n",sHead->id, sHead->approach, sHead->turnType);
		printf("East Intersection Id: %d Approach: %d Turn Type: %d\n",eHead->id, eHead->approach, eHead->turnType);
		printf("West Intersection Id: %d Approach: %d Turn Type: %d\n",wHead->id, wHead->approach, wHead->turnType);

    int previousHead = 0;
    for(int i = 0; i < MAX_CARS; i++){
      printf("%d\n", i);
      //printf("%d\n", previousHead);
      switch (previousHead) {
        case 1:
          if(nHead != NULL)
            nHead = nHead->next;
          break;
        case 2:
          if(sHead != NULL)
            sHead = sHead->next;
          break;
        case 3:
          if(eHead != NULL)
            eHead = eHead->next;
          break;
        case 4:
          if(wHead != NULL)
            wHead = wHead->next;
          break;
      }
      if(nHead == NULL){
        nCost = 9999;
      } else if(nHead->turnType==Right){
        //NEEDS NW
        nCost=1;
      } else if(nHead->turnType==Straight){
        //NEEDS NW AND SW
        nCost=2;
      } else if(nHead->turnType==Left){
        nCost=3;
      }
      if(sHead == NULL){
        sCost = 9999;
      } else if(sHead->turnType==Right){
        sCost=1;
      } else if(sHead->turnType==Straight){
        sCost=2;
      } else if(sHead->turnType==Left){
        sCost=3;
      }
      if(eHead == NULL){
        eCost = 9999;
      } else if(eHead->turnType==Right){
        eCost=1;
      } else if(eHead->turnType==Straight){
        eCost=2;
      } else if(eHead->turnType==Left){
        eCost=3;
      }
      if(wHead == NULL){
        wCost = 9999;
      } else if(wHead->turnType==Right){
        wCost=1;
      } else if(wHead->turnType==Straight){
        wCost=2;
      } else if(wHead->turnType==Left){
        wCost=3;
      }
      printf("N Cost: %d S Cost: %d E Cost: %d W Cost: %d\n", nCost, sCost, eCost, wCost);
      //printf("N id: %d S id: %d E id: %d W id: %d\n", nHead->id, sHead->id, eHead->id, wHead->id);
      int min = 9998;
      int comingFrom = 0;
      if(nCost < min){
        min = nCost;
        comingFrom = 1;
      } else if(nCost == min){
        if(whoPasses() == 1){
          min = nCost;
          comingFrom = 1;
        }
      }
      if(sCost < min){
        min = sCost;
        comingFrom = 2;
      } else if(sCost == min){
        if(whoPasses() == 1){
          min = sCost;
          comingFrom = 2;
        }
      }
      if(eCost < min){
        min = eCost;
        comingFrom = 3;
      } else if(eCost == min){
        if(whoPasses() == 1){
          min = eCost;
          comingFrom = 3;
        }
      }
      if(wCost < min){
        min = wCost;
        comingFrom = 4;
      } else if(wCost == min){
        if(whoPasses() == 1){
          min = wCost;
          comingFrom = 4;
        }
      }
      switch (comingFrom) {
        case 1:
          previousHead = 1;
          //printf("id del que se va %d\n", nHead->id);
          pthread_create(&car1_t, NULL, (void *) &handleThreads, (void *) nHead);
          break;
        case 2:
          previousHead = 2;
          //printf("id del que se va %d\n", sHead->id);
          pthread_create(&car2_t, NULL, (void *) &handleThreads, (void *) sHead);
          break;
        case 3:
          previousHead = 3;
          //printf("id del que se va %d\n", eHead->id);
          pthread_create(&car3_t, NULL, (void *) &handleThreads, (void *) eHead);
          break;
        case 4:
          previousHead = 4;
          //printf("id del que se va %d\n", wHead->id);
          pthread_create(&car4_t, NULL, (void *) &handleThreads, (void *) wHead);
          break;
      }
      usleep(30);
    }

		// //LEFT
		// if(nHead->turnType==Left){
		// 	//NEEDS NW, SW AND SE

		// }
		// if(sHead->turnType==Left){
		// 	//NEEDS SE, NE, AND NW
		// }
		// if(eHead->turnType==Left){
		// 	//NEEDS NE, NW, AND SW
		// }
		// if(wHead->turnType==Left){
		// 	//NEEDS SW, SE, AND NE
		// }



		//Intersection has 4 cars @ a time
		//If Right have to wait on right close corner semiphore
		//If Straight have to wait on right close corner, and far right corner
		//If Left have to wait on right close corner, far right corner, and far left corner semiphores
    pthread_join(car1_t, NULL);
    pthread_join(car2_t, NULL);
    pthread_join(car3_t, NULL);
    pthread_join(car4_t, NULL);
}

void handleThreads(car *car_info){
  //int val;
  sem_wait(&nw);
  //sem_getvalue(&nw,&val);
  //printf("SEM INSIDE %d\n", val);
  printf("GLOBAL COUNT: %d\n", globalCount);
  globalCount++;
  printf("Car #%d approaching intesection\n", car_info->id);
  printf("Approach direction: ");
  printDirections(car_info->approach);
  printf("Destination direction: ");
  printTurn(car_info->turnType);
  printf("Car #%d leaving intesection\n", car_info->id);
  sem_post(&nw);
  //sem_getvalue(&nw,&val);
  //printf("SEM OUTIE %d\n", val);
  return;
}

void printDirections(approach a){
  if(a == N){
    printf("North\n");
  } else if(a == S){
    printf("South\n");
  } else if(a == E){
    printf("East\n");
  } else if(a == W){
    printf("West\n");
  }
}

void printTurn(turnType t){
  if(t == Straight){
    printf("Straight\n");
  } else if(t == Right){
    printf("Right\n");
  } else if(t == Left){
    printf("Left\n");
  }
}

int whoPasses(){
  int r = (rand() % (1))+1;
  return r;
}

int main() {
	simulateIntersection();
	return 0;
}
