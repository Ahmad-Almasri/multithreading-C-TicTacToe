/*

--> Name : Ahmad Almasri -- University of Calgary -- 30114233

--> Multithreading programming in c [Pthreads]

--> This program simulates Tic-Tac-Toe game
  using three threads <Main_thread, player one and two threads>

*/


#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"
#include "unistd.h"
//------------------------------------  

// == struct & struct shared play ==    
struct shared
{
  char *board;
  int turn;
}play;

//------------------------------------ 

// == declare functions ==
void *mainThread(void *);
void initialize();
void display();
void decideMove(int);
void *worker(void *);
bool checkEmpty(int);
bool thereIsWinner(char *);
bool boardIsFull(char );
int setTurn(int);

//------------------------------------

// == START MAIN FUNCTION ==
int main(int argc, char const *argv[]) {
  
  pthread_t main_t;      
  pthread_create(&main_t, NULL, mainThread, NULL);
  pthread_join(main_t, NULL);

  return 0;
}

//------------------------------------

void * mainThread(void * var){

  // call :: initialize
  initialize();
  // init :: checkFor variable
  char checkFor = '-';
  // set :: nextPlayer = 1
  int nextPlayer=1;
  // set :: currentTurn = 0
  play.turn = 0;
  // declare :: two threads
  pthread_t playerOne, playerTwo;
  // threads :: ids
  int tId1 = 1;
  int tId2 = 2;
  // seeds :: rand()
  srand(time(NULL));
  // create :: two threads
  pthread_create(&playerOne, NULL, worker, &tId1);
  pthread_create(&playerTwo, NULL, worker, &tId2);
  // :: infinite loop
  while(1)
  {
    // :: wait until the turn is zero
    while(play.turn != 0);
    // :: call display
    display();
    // :: check the game status
    if(thereIsWinner(&checkFor)||boardIsFull(checkFor))
    {
      // cancel :: two threads
      pthread_cancel(playerOne);
      pthread_cancel(playerTwo);
      // print :: result
      checkFor == '-' ? printf("IT IS A :: DRAW ::\n"):printf("PLAYER %c IS THE :: WINNER ::\n", checkFor);
      // :: stop the infinite loop
      break;
    }
    else
    {
      // :: choose nextPlayer
      nextPlayer = setTurn(nextPlayer);
    }

  }
}
//------------------------------------
void initialize(){
  // :: initialize 9 bytes array
  play.board = (char *) malloc (9*sizeof(char));
  // :: filling the array
  for(int i = 0; i < 9; i++){
    play.board[i] = '-';
  }
}
//------------------------------------
void display(){
  printf(" TIC TAC TOE\n");
  printf("------------- \n");
  for(int i = 0; i < 9; i+=3){
    printf("| %c | %c | %c |\n",play.board[i], play.board[i+1], play.board[i+2]);
    printf("------------- \n");
  }
}
//------------------------------------
void *worker(void * Playerid)
{
  // :: get id of the thread
  int id =* (int*) Playerid, randNumber;
  while(1)
  {
    // :: wait until your turn
    while (play.turn != id);
    // :: 0 <= get rand <= 8 :: [0,8]
    randNumber = rand() % 9;
    if(checkEmpty(randNumber)){
      decideMove(randNumber);
    }
   }
}
//------------------------------------
bool checkEmpty(int index){
  // :: if empty --> true else false
  if(play.board[index]=='-') return true; return false;
}
//------------------------------------
void decideMove(int index){
  // :: if turn = 1 --> X else O
  //printf("--> index = %d & id = %d \n", index, play.turn);
  if(play.turn == 1) play.board[index]='X';else play.board[index]='O';
  // :: set turn to the mainThread
  play.turn = 0;
  return;
}
//------------------------------------
// :: decide the nextPlayer function
int setTurn(int nextPlayer)
{
  if(nextPlayer == 1)
  {
    play.turn = 1;
    // :: next player will be 2
    return 2;
  }
  else
  {
    play.turn =  2;
    // :: next player will be 1
    return 1;
  }
}
//------------------------------------
bool thereIsWinner(char *check)
{
  // cases of wining the game:

  // :: Vertical :: <0,1,2> , <3,4,5> , <6,7,8>
  char checkFor = 'X';
  for(int i=0; i<2; i++){
    for(int j=0; j<9; j+=3){
      if(play.board[j] == checkFor && play.board[j+1] == checkFor && play.board[j+2] == checkFor){
        *check = checkFor;
        return true;
      }
    }
    checkFor = 'O';
  }
  // :: Horizental :: <0,3,6> , <1,4,7> , <2,5,8>
  checkFor = 'X';
  for(int i=0; i<2; i++){
    for(int j=0; j<3; j++){
      if(play.board[j] == checkFor && play.board[j+3] == checkFor && play.board[j+6] == checkFor){
        *check = checkFor;
        return true;
      }
    }
    checkFor = 'O';
  }
  // :: Diagonal :: <0,4,8> , <2,4,6>
  checkFor = 'X';
  for(int i=0; i<2; i++){
    if(play.board[0] == checkFor && play.board[4] == checkFor && play.board[8] == checkFor){
      *check = checkFor;
      return true;
    }else if(play.board[2] == checkFor && play.board[4] == checkFor && play.board[6] == checkFor){
      *check = checkFor;
      return true;
    }
    checkFor = 'O';
  }
  // if three is NO WINNER :: set :: checkFor (On the main) = '-' & return false
  *check = '-';
  return false;
}
//------------------------------------
bool boardIsFull(char checkFor){
  // IF :: checkFor is equal to X || O
  // THEN :: there is a winner :: return false
  if(checkFor != '-'){
    return false;
  }else{
    // :: counter
    int i = 0;
    while(1)
    {
      if(i < 9 && play.board[i]=='-')
          // :: is not full
          return false;
      i++;
      // :: no more cells on the board
      // :: all of them are Xs || Os
      if(i==9)
          // :: is full
          return true;
    }
  }
}
//------------------------------------
