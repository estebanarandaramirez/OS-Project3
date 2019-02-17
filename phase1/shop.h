enum customer {pirate, ninja};
void Enter(enum customer c); // Enter bathroom but wait if occupied by opposite gender
                      // set state
void Leave(void); // Leave bathroom and set state to vacant if this is the last
                  // thread to leave
void Initialize(int numTeams);
void Finalize(double grossRevenue);
void passWaitTime(double wt);

#define MAX_TIME 1440 //number of seconds to simulate a day
#define MAX_VISITS 20 //max number of visits possible
