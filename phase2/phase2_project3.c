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

void *Intersection(void* arg){


}

void simulateIntersection(){
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
    	car *temp1;
    	temp1=malloc(sizeof(car));
    	temp1=nHead;
    	for(int i=0; i<=nCount-1; i++){
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
		temp1=sHead;
		for(int i=0; i<=sCount-1; i++){
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
		temp1=eHead;
		for(int i=0; i<=eCount-1; i++){
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
		temp1=wHead;
		for(int i=0; i<=wCount-1; i++){
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


		car *nCar=nHead;
		car *sCar=sHead;
		car *eCar=eHead;
		car *wCar=wHead;
		intersectionQueue *inter;

		nCar = malloc(sizeof(car));
		sCar = malloc(sizeof(car));
		eCar = malloc(sizeof(car));
		wCar = malloc(sizeof(car));
		inter = malloc(sizeof(intersectionQueue));
		// sem_init(&nw, 0, 1);
		// sem_init(&ne, 0, 1);
		// sem_init(&sw, 0, 1);
		// sem_init(&se, 0, 1);
		// pthread_t car1_t, car2_t, car3_t, car4_t;
		// pthread_create(&car1_t, NULL, nHandle, NULL);
		// pthread_create(&car2_t, NULL, sHandle, NULL);
		// pthread_create(&car3_t, NULL, eHandle, NULL);
		// pthread_create(&car4_t, NULL, wHandle, NULL);

		int nCost=3;
		int sCost=3;
		int eCost=3;
		int wCost=3;

		printf("North Intersection Id: %d Approach: %d Turn Type: %d\n",nHead->id, nHead->approach, nHead->turnType);
		printf("South Intersection Id: %d Approach: %d Turn Type: %d\n",sCar->id, sCar->approach, sCar->turnType);
		printf("East Intersection Id: %d Approach: %d Turn Type: %d\n",eCar->id, eCar->approach, eCar->turnType);
		printf("West Intersection Id: %d Approach: %d Turn Type: %d\n",wCar->id, wCar->approach, wCar->turnType);

		//Loop
		//RIGHT
		if(nCar->turnType==Right){
			//NEEDS NW
			nCost=1;
		}
		if(sCar->turnType==Right){
			//NEEDS SE
			sCost=1;
		}
		if(eCar->turnType==Right){
			//NEEDS NE
			eCost=1;
		}
		if(wCar->turnType==Right){
			//NEEDS SW
			wCost=1;
		}

		//STRAIGHT
		if(nCar->turnType==Straight){
			//NEEDS NW AND SW
			nCost=2;
		}
		if(sCar->turnType==Straight){
			//NEEDS SE AND NE
			sCost=2;
		}
		if(eCar->turnType==Straight){
			//NEEDS NE AND NW
			eCost=2;
		}
		if(wCar->turnType==Straight){
			//NEEDS SW AND SE
			wCost=2;
		}

		printf("N Cost: %d S Cost: %d E Cost: %d W Cost: %d\n", nCost, sCost, eCost, wCost);



		// //LEFT
		// if(nCar->turnType==Left){
		// 	//NEEDS NW, SW AND SE

		// }
		// if(sCar->turnType==Left){
		// 	//NEEDS SE, NE, AND NW
		// }
		// if(eCar->turnType==Left){
		// 	//NEEDS NE, NW, AND SW
		// }
		// if(wCar->turnType==Left){
		// 	//NEEDS SW, SE, AND NE
		// }



		//Intersection has 4 cars @ a time
		//If Right have to wait on right close corner semiphore
		//If Straight have to wait on right close corner, and far right corner
		//If Left have to wait on right close corner, far right corner, and far left corner semiphores

}

int main() {
	simulateIntersection();
	return 0;
}
