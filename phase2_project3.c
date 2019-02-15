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

struct car{
  int id;
  int approach; //0=North 1=South 2=East 3=West
  int turnType; //0=Straight 1=Right 2=Left
};

struct queue{
  struct car list[MAX_CARS];
};

//
// struct queue{
//   int approach;//0=North 1=South 2=East 3=West
//   struct car;
// };

void* thread(void* arg){
  sem_wait(&nw);
  printf("\nEntered..\n");
  sleep(4);
  printf("\nJust Exiting...\n");
  sem_post(&nw);
}

void randomCars(){
  struct car *temp;
  struct queue *n;
  struct queue *s;
  struct queue *e;
  struct queue *w;
  temp = (struct car*)malloc(sizeof(struct car));
  n = (struct queue*)malloc(sizeof(struct queue));
  s = (struct queue*)malloc(sizeof(struct queue));
  e = (struct queue*)malloc(sizeof(struct queue));
  w = (struct queue*)malloc(sizeof(struct queue));
  srand(time(NULL));
  for(int i=0; i<MAX_CARS; i++){
    temp->id=i;
    temp->approach=rand()%4;
    temp->turnType=rand()%3;
    printf("Id: %d Approach: %d Turn: %d\n",temp->id,temp->approach,temp->turnType);
    switch(temp->approach){
      case 0:
        n->list[i]=*temp;
        printf("North added\n");
        break;
      case 1:
        s->list[i]=*temp;
        printf("South added\n");
        break;
      case 2:
        e->list[i]=*temp;
        printf("East added\n");
        break;
      case 3:
        w->list[i]=*temp;
        printf("West added\n");
        break;
      default:
        printf("Something went horribly wrong");
        break;
    }
  }
  free(temp);
}

int main() {
  randomCars();
  // sem_init(&nw, 0, 1);
  // sem_init(&ne, 0, 1);
  // sem_init(&sw, 0, 1);
  // sem_init(&se, 0, 1);
  // pthread_t c1, c2, c3, c4;
  // pthread_create(&c1, NULL, thread, NULL);
  // sleep(2);
  // pthread_create(&c2, NULL, thread, NULL);
  // pthread_join(c1,NULL);
  // pthread_join(c2, NULL);
  // sem_destroy(&nw);
  return 0;
}
