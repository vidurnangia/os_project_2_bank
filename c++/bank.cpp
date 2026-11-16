#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <queue>
#include <string>
#include "semaphore.h"
using namespace std;

const int NUM_TELLERS = 3; //.we have 3 tellers
const int NUM_CUSTOMERS = 50; //and 50 customers

//mutex, ensure only one thread can print at a time
Semaphore printlock(1);

//only 2 customers should pass at a time
Semaphore doorSem(2);

//safe can only hold 2 tellers at once at most
Semaphore safeSem(2);

//
Semaphore managerSem(1);

/*
add the remaining semaphores per specs*/

//init data structures (need for waiting tellers, id of customer at teller)
//and also transaction
queue<int> readyQueue;
int tellerCustomer[NUM_TELLERS];
Txn tellerTxn[NUM_TELLERS];

//print function, void loop


//teller thread
void teller(int id) {

}

//customer thread
void customer (int id) {
    //randomlyu pick transaction and wait
    //wait for bank to open, 2 in at a time
    //get custome rin line aand choose the teller free for the longest
}