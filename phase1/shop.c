#include "shop.h"
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

struct teamStat{
  double last_time;
  double time_free;
  double time_busy;
};

struct costumingShop{
  int teams; // num of costuming teams
  struct teamStat ts[4];
  int freeTeams; // num of costuming teams that are free
  int users; // num of current users in shop
  enum customer cust; // type of customer currently in shop
  int pirates; //num of pirates inside
  int ninjas; //num of ninjas inside
  int usages; // number of total usages
  int queue; // number of people currently in the queue
  int qcount; // number of changes to the length of queue
  int ucount; // number of changes to people in shop
  double time_free; // total time free
  double time_busy; // total time busy
  double last_time; // last time the shop was taken by a customer or left unoccupied
  double start_time; // initialization time
  double avg_users; // ave users
  double avg_queue; // average people in queue
  double grossRevenue; // gross revenue
  double goldePerVisit; // gross revenue / usages
  double totalProfits; // gross revenue - money owed to costuming teams
};

double getmytime2();
double calc_avg2(double last_ave, int count, int new_data);

struct costumingShop shop; // global shop object
pthread_mutex_t shop_lock = PTHREAD_MUTEX_INITIALIZER; // lock for altering shop
pthread_mutex_t exit_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pir = PTHREAD_COND_INITIALIZER; // condition for pirates to enter
pthread_cond_t nin = PTHREAD_COND_INITIALIZER; // condition for ninjas to enter
pthread_cond_t tms = PTHREAD_COND_INITIALIZER;

void Initialize(int numTeams){
  shop.teams = numTeams; //Initialize shop
  for(int i = 0; i < numTeams; i++){
    shop.ts[i].last_time = getmytime2();
    shop.ts[i].time_free = 0;
    shop.ts[i].time_busy = 0;
  }
  shop.freeTeams = numTeams;
  shop.users = 0;
  shop.usages = 0;
  shop.pirates = 0;
  shop.ninjas = 0;
  shop.queue = 0;
  shop.qcount = 0;
  shop.ucount = 0;
  shop.time_free = 0;
  shop.time_busy = 0;
  shop.last_time = getmytime2(); // sets times
  shop.start_time = shop.last_time;
  shop.avg_users = 0;
  shop.avg_queue = 0;
  shop.grossRevenue = 0;
  shop.goldePerVisit = 0;
  shop.totalProfits = 0;
}

void Finalize(double grossRevenue){
  shop.grossRevenue = grossRevenue; // Calculate some stats
  shop.goldePerVisit = shop.grossRevenue/shop.usages;
  shop.totalProfits = shop.grossRevenue - (5 * shop.teams);
  printf("Shop Stats\n");
  printf("Total number of usages: %d\n", shop.usages); // Print stats
  printf("Shop free time: %lf\n", shop.time_free);
  printf("Shop occupied time: %lf\n", shop.time_busy);
  for(int i = 0; i < shop.teams; i++){
    printf("Team %d free time: %lf\n", i+1, shop.ts[i].time_free);
    printf("Team %d occupied time: %lf\n", i+1, shop.ts[i].time_busy);
  }
  printf("Average length of queue: %lf\n", shop.avg_queue);
  printf("Average users at the same time: %lf\n", shop.avg_users);
  printf("Gross Revenue: %lf\n", shop.grossRevenue);
  printf("Average Gold Per Visit: %lf\n", shop.goldePerVisit);
  printf("Total Profits: %lf\n", shop.totalProfits);
}

void Enter(enum customer c){
  double time1, time2, waitTime;
  double tim;
  int q = 1; // flag for whether the thread has joined the queue
  pthread_mutex_lock(&shop_lock);
  if (shop.users == 0) // If there are no users we can set the customer type
    shop.cust = c;
  time1 = getmytime2();
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
  while(shop.freeTeams <= 0 && shop.cust == c){
    //printf("antes %d\n", shop.freeTeams);
    shop.queue++; // Update number of people in queue
    shop.qcount++; // update changes of length of queue
    q = 0;
    shop.avg_queue = calc_avg2(shop.avg_queue, shop.qcount, shop.queue); // Calcute new ave queue len
    assert (shop.freeTeams <= 0);
    pthread_cond_wait(&tms, &shop_lock);
    //printf("despues %d\n", shop.freeTeams);
  }
  time2 = getmytime2();
  waitTime = time2-time1;
  passWaitTime(waitTime);
  if (shop.users == 0) {
    tim = getmytime2();
    shop.time_free += tim-shop.last_time; // Shop was empty, so update free time
    shop.last_time = tim;
    shop.ts[0].time_free += tim-shop.ts[0].last_time;
    shop.ts[0].last_time = tim;
    shop.ts[1].time_free += tim-shop.ts[1].last_time;
    shop.ts[1].last_time = tim;
    shop.ts[2].time_free += tim-shop.ts[2].last_time;
    shop.ts[2].last_time = tim;
    shop.ts[3].time_free += tim-shop.ts[3].last_time;
    shop.ts[3].last_time = tim;
  } else if(shop.users == 1) {
    tim = getmytime2();
    shop.ts[1].time_free += tim-shop.ts[1].last_time;
    shop.ts[1].last_time = tim;
    shop.ts[2].time_free += tim-shop.ts[2].last_time;
    shop.ts[2].last_time = tim;
    shop.ts[3].time_free += tim-shop.ts[3].last_time;
    shop.ts[3].last_time = tim;
  } else if(shop.users == 2) {
    tim = getmytime2();
    shop.ts[2].time_free += tim-shop.ts[2].last_time;
    shop.ts[2].last_time = tim;
    shop.ts[3].time_free += tim-shop.ts[3].last_time;
    shop.ts[3].last_time = tim;
  } else if(shop.users == 3) {
    tim = getmytime2();
    shop.ts[3].time_free += tim-shop.ts[3].last_time;
    shop.ts[3].last_time = tim;
  }
  if (q == 0){
    shop.queue--; // Recaclulate average people in the queue
    shop.qcount++; // update changes of length of queue
    //assert(c == shop.cust);
    shop.avg_queue = calc_avg2(shop.avg_queue, shop.qcount, shop.queue); // recalculate avg queue
  }
  shop.freeTeams--;
  shop.users++; // increment num of users
  //printf("entrando %d\n", shop.users);
  shop.usages++; // increment shop usages
  shop.ucount++;
  shop.avg_users = calc_avg2(shop.avg_users, shop.ucount, shop.users); // recalculate avg number of users
  if (c==pirate) // update number of each type of customer in shop
    shop.pirates++;
  else if (c==ninja)
    shop.ninjas++;
  //assert((shop.pirates == 0) || (shop.ninjas == 0)); // check that both type of customers are not in shop
  //assert((shop.pirates == shop.users) || (shop.ninjas == shop.users)); // check that all users in shop are one customer type
  pthread_mutex_unlock(&shop_lock); // release lock
}

void Leave(){
  double tim;
  pthread_mutex_lock(&shop_lock);
  if(shop.freeTeams <= 0){
    pthread_cond_broadcast(&tms);
  }
  shop.freeTeams++;
  shop.users--;
  //printf("saliendo %d\n", shop.users);
  shop.ucount++;
  shop.avg_users = calc_avg2(shop.avg_users, shop.ucount, shop.users); // recalculate avg number of users
  if (shop.cust == pirate)
    shop.pirates--;
  else if (shop.cust == ninja)
    shop.ninjas--;
  //assert((shop.pirates == 0) || (shop.ninjas== 0));
  //assert((shop.pirates == shop.users) || (shop.ninjas == shop.users));
  if(shop.users == 0){ // check if this thread is the last to leave
    tim = getmytime2();
    shop.time_busy += tim-shop.last_time; // Shop was used, so update busy time
    shop.last_time = tim;
    shop.ts[0].time_busy += tim-shop.ts[0].last_time;
    shop.ts[0].last_time = tim;
    if (shop.cust == ninja)
      pthread_cond_broadcast(&pir);
    else if (shop.cust == pirate)
      pthread_cond_broadcast(&nin);
  } else if(shop.users == 1) {
    tim = getmytime2();
    shop.ts[1].time_busy += tim-shop.ts[1].last_time;
    shop.ts[1].last_time = tim;
  } else if(shop.users == 2) {
    tim = getmytime2();
    shop.ts[2].time_busy += tim-shop.ts[2].last_time;
    shop.ts[2].last_time = tim;
  } else if(shop.users == 3) {
    tim = getmytime2();
    shop.ts[3].time_busy += tim-shop.ts[3].last_time;
    shop.ts[3].last_time = tim;
  }
  pthread_mutex_unlock(&shop_lock); // release lock
}

double getmytime2() { // get time
  struct timeval tim;
  gettimeofday(&tim,0);
  double t = tim.tv_sec*1000+tim.tv_usec/1000;
  t = t/1000;
  return t;
}

double calc_avg2(double last_ave, int count, int new_data) { // recalculate average with new data
  double new_ave, ratio;
  ratio = (double) count-1;
  ratio = ratio/count;
  new_ave = last_ave*ratio;
  ratio = (double) new_data/count;
  new_ave += ratio;
  // ratio = last_ave * (double)(count - 1);
  // ratio += new_data;
  // new_ave = ratio/count;
  return new_ave;
}
