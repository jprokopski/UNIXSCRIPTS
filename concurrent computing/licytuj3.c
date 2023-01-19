#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
 
#define MEM_SIZ 4096
#define N_AGENTS 20
#define N_ITEMS 20
#define OPENING_BID 100
#define MAX_RAISE 10
#define BIDDING_ROUNDS 1000000
 
struct wspolna {
  long int BIDS[N_ITEMS];
  long int NBIDS[N_ITEMS];
};
 
int main() {
  struct wspolna *ptr;
  int shmid = shmget(259616, sizeof(struct wspolna), 0600 | IPC_CREAT);
  if (shmid == -1) {
    perror("porazka shmget");
    exit(errno);
  }
  ptr = (struct wspolna*)shmat(shmid, 0, 0);
  if (ptr == (struct wspolna*)-1) {
    perror("porazka shmat");
    exit(errno);
  }
 
  long int SUM = 0;
  long int SUM_BIDS = 0;
  srand(getpid());
  for(int i = 0; i < N_ITEMS; i++) {
    ptr -> BIDS[i] = OPENING_BID;
    ptr -> NBIDS[i] = 0;
  }
 
  for(int i = 0; i < N_AGENTS; i++) {
    if (fork() == 0) {
      for(int i = 0; i < BIDDING_ROUNDS; i++) {
          int raise  = rand() % (MAX_RAISE + 1);
          int item = rand() % N_ITEMS;
          ptr -> BIDS[item] += raise;
          ptr -> NBIDS[item] += 1;
      }
    exit(0);
    }
  }
  while (waitpid(-1, 0, 0) > 0);
 
  for(int i = 0; i < N_ITEMS; i++) {
    SUM += ptr -> BIDS[i];
    SUM_BIDS += ptr -> NBIDS[i];
    printf("Cena przedmiotu %d wynosi %d, ilosc podbic: %d\n", i + 1, ptr -> BIDS[i], ptr -> NBIDS[i]);
  }
 
  printf("Suma cen wynosi %ld\n", SUM);
  printf("Liczba podbic wynosi %ld\n", SUM_BIDS);
  return 0;
}