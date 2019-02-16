#include "shop.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

struct costumingShop{
  int teams;
  int freeTeams;
  int users; // num of current users in bath
  enum customer cust; // gender of current users
  int pirates; //num of males and females inside
  int ninjas;
  int usages; // number of total usages
  int queue; // number of people currently in the queue
  int qcount; // number of changes to the length of queue
  int ucount; // number of changes to people in bath
  double time_vacant; // total time vacant in ms
  double time_used; // total time used in ms
  double last_time; // last time the bathroom was taken by a gender or left unoccupied
  double start_time; // initialization time
  double avg_users; // ave users
  double avg_queue; // average people in queue
  double grossRevenue;
  double goldePerVisit;
  double totalProfits;
};

double getmytime2();
double calc_avg2(double last_ave, int count, int new_data);
double getmytime();

struct costumingShop shop; // global bathroom object
pthread_mutex_t shop_lock = PTHREAD_MUTEX_INITIALIZER; // lock for altering bath
pthread_cond_t pir = PTHREAD_COND_INITIALIZER; // condition for males to enter
pthread_cond_t nin = PTHREAD_COND_INITIALIZER; // condition for females to enter

void Initialize(int numTeams){
  shop.teams = numTeams;
  shop.freeTeams = numTeams;
  shop.users = 0; // Set everything to 0
  shop.usages = 0;
  shop.pirates = 0;
  shop.ninjas = 0;
  shop.queue = 0;
  shop.qcount = 0;
  shop.ucount = 0;
  shop.time_vacant = 0;
  shop.time_used = 0;
  shop.last_time = getmytime2(); // sets times
  shop.start_time = shop.last_time;
  shop.avg_users = 0;
  shop.avg_queue = 0;
  shop.grossRevenue = 0;
  shop.goldePerVisit = 0;
  shop.totalProfits = 0;
}

void Finalize(double grossRevenue){
  shop.grossRevenue = grossRevenue;
  shop.goldePerVisit = shop.grossRevenue/shop.usages;
  shop.totalProfits = shop.grossRevenue - (5 * shop.teams);
  printf("Shop Stats\n");
  printf("Total number of usages: %d\n", shop.usages); // Print stats
  printf("Total vacant time (ms): %lf\n", shop.time_vacant);
  printf("Total occupied time (ms): %lf\n", shop.time_used);
  printf("Average length of queue: %lf\n", shop.avg_queue);
  printf("Average users at the same time: %lf\n", shop.avg_users);
  printf("Gross Revenue: %lf\n", shop.grossRevenue);
  printf("Average Gold Per Visit: %lf\n", shop.goldePerVisit);
  printf("Total Profits: %lf\n", shop.totalProfits);
}

void Enter(enum customer c){
  printf("FREE TEAMS%d\n", shop.freeTeams);
  double time1, time2, waitTime;
  double tim;
  int q = 1; // flag for whether the thread has joined the queue
  pthread_mutex_lock(&shop_lock);
  if (shop.users == 0) // If there are no users we can set the gender
    shop.cust = c;
  time1 = getmytime();
  while(shop.cust != c){ // There is someone in the bathroom so join the queue and wait
    shop.queue++; // Update number of people in queue
    shop.qcount++; // update changes of length of queue
    q = 0;
    shop.avg_queue = calc_avg2(shop.avg_queue, shop.qcount, shop.queue); // Calcute new ave queue len
    assert (c!= shop.cust);
    if (c == pirate) // wait for condition variables
      pthread_cond_wait(&pir, &shop_lock);
    else if (c == ninja)
      pthread_cond_wait(&nin, &shop_lock);
    if (shop.users == 0) // the thread was woken up so it has to check whether the bathroom is empty
      shop.cust = c;
  }
  while(shop.freeTeams < 0){
    shop.queue++; // Update number of people in queue
    shop.qcount++; // update changes of length of queue
    q = 0;
    shop.avg_queue = calc_avg2(shop.avg_queue, shop.qcount, shop.queue); // Calcute new ave queue len
    assert (shop.freeTeams < 0);
    //printf("FREE TEAMS%d\n", shop.freeTeams);
  }
  time2 = getmytime();
  waitTime = time2-time1;
  passWaitTime(waitTime);
  if (shop.users == 0) {
    tim = getmytime2();
    shop.time_vacant += tim-shop.last_time; // Bathroom was empty, so update vacant time
    shop.last_time = tim;
  }
  if (q == 0){
    shop.queue--; // Recaclulate average people in the queue
    shop.qcount++; // update changes of length of queue
    assert(c == shop.cust);
    shop.avg_queue = calc_avg2(shop.avg_queue, shop.qcount, shop.queue); // recalculate avg queue
  }
  shop.freeTeams--;
  shop.users++; // increment no of users
  shop.usages++; // increment bath usages
  shop.ucount++;
  shop.avg_users = calc_avg2(shop.avg_users, shop.ucount, shop.users); // recalculate avg number of users
  if (c==pirate) // update number of each gender in bath
    shop.pirates++;
  else if (c==ninja)
    shop.ninjas++;
  assert((shop.pirates == 0) || (shop.ninjas == 0)); // check that both genders are not in bath
  assert((shop.pirates == shop.users) || (shop.ninjas == shop.users)); // check that all users in bath are one gender
  pthread_mutex_unlock(&shop_lock); // release lock
}

void Leave(){
  double tim;
  pthread_mutex_lock(&shop_lock);
  if(shop.freeTeams == 0){
    if (shop.cust == ninja)
      pthread_cond_broadcast(&nin);
    else if (shop.cust == pirate)
      pthread_cond_broadcast(&pir);
  }
  shop.freeTeams++;
  shop.users--;
  shop.ucount++; // increment bath usages
  shop.avg_users = calc_avg2(shop.avg_users, shop.ucount, shop.users); // recalculate avg number of users
  if (shop.cust == pirate)
    shop.pirates--;
  else if (shop.cust == ninja)
    shop.ninjas--;
  assert((shop.pirates == 0) || (shop.ninjas== 0));
  assert((shop.pirates == shop.users) || (shop.ninjas == shop.users));
  if(shop.users == 0){ // check if this thread is the last to leave
    if (shop.cust == ninja)
      pthread_cond_broadcast(&pir);
    else if (shop.cust == pirate)
      pthread_cond_broadcast(&nin);
    tim = getmytime2();
    shop.time_used += tim-shop.last_time; // Bathroom was used, so update used time
    shop.last_time = tim;
  }
  //assert((bath.qmales == 0) || (bath.qfemales== 0)); // check that both genders are not in queue
  //assert((bath.qmales == bath.queue) || (bath.qfemales == bath.queue)); // check that number of people in queue is just one gender
  pthread_mutex_unlock(&shop_lock); // release lock
}

double getmytime2() { // get time in ms as double
  struct timeval tim;
  gettimeofday(&tim,0);
  double t = tim.tv_sec*1000+tim.tv_usec/1000;
  return t;
}

double calc_avg2(double last_ave, int count, int new_data) { // recalculate average with new data
  double new_ave, ratio;
  ratio = (double) count-1;
  ratio = ratio/count;
  new_ave = last_ave*ratio;
  ratio = (double) new_data/count;
  new_ave += ratio;
  return new_ave;
}
