#include <iostream>
#include <chrono>
#include <random>
#include <queue>
#include <string>
#include "semaphore.h"
using namespace std;

const int NUM_TELLERS = 3; //.we have 3 tellers
const int NUM_CUSTOMERS = 50; //and 50 customers

enum Txn {WITHDRAW, DEPOSIT}; //transaction types

//mutex, ensure only one thread can print at a time
Semaphore printlock(1);

//only 2 customers should pass at a time
Semaphore doorSem(2);

//safe can only hold 2 tellers at once at most
Semaphore safeSem(2);

//
Semaphore managerSem(1);
Semaphore readyTellers(0);
Semaphore queueLock(1);
Semaphore tellersReady(0);
Semaphore bankOpen(0);

//interaction semaphores in order of transaction
Semaphore customerArrived[NUM_TELLERS];
Semaphore askedTxn[NUM_TELLERS];
Semaphore gaveTxn[NUM_TELLERS];

Semaphore txnDone[NUM_TELLERS];       
Semaphore custLeft[NUM_TELLERS]; //customer leaves

/*
add the remaining semaphores per specs*/

//init data structures (need for waiting tellers, id of customer at teller)
//and also transaction
queue<int> readyQueue;
int tellerCustomer[NUM_TELLERS];
Txn tellerTxn[NUM_TELLERS];

//print function, void loop


//
int randRange(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}
//teller thread
void teller(int id) {
bool first = true;
while(true) {
    print("Teller", id, "Customer", -1, "ready to serve");
    queueLock.wait();
    readyQueue.push(id);
    queueLock.signal();
    readyTellers.signal();
    if (first)
    {
       tellersReady.signal();
       first = false;        
    }

    //make customer
    customerArrived[id].wait();
    int cust = tellerCustomer[id];
    if (cust < 0) {
        print("Teller", id, "Customer", cust, "leaving for the day");
        return;
        }
    print("Teller", id, "Customer", cust, "serving a customer");

    //ask & wait for transaction
    print("Teller", id, "Customer", cust, "asks for transaction");
    askedTxn[id].signal();
    gaveTxn[id].wait();
    Txn t = tellerTxn[id];
    print("Teller", id, "Customer", cust,
            t == WITHDRAW ? "receives withdraw transaction" : 
            "receives deposit transaction");

    //withdrawals needing manager consent
    if (t == WITHDRAW) {
        print("Teller", id, "Customer", cust, "getting the manager");
        managerSem.wait();
        print("Teller", id, "Customer", cust, "getting approval");
        sleepMs(randRange(rng, 5, 30));
            print("Teller", id, "Customer", cust, "got manager's permission");
            managerSem.signal();
    }

    //safe requirements
    print("Teller", id, "Customer", cust, "going to safe");
        safeSem.wait(); //enters slot
        print("Teller", id, "Customer", cust, "in safe&performing transaction");
        sleepMs(randRange(rng, 10, 50)); //random range 10-50ms
        print("Teller", id, "Customer", cust, "done in the safe, leaving the safe");
        safeSem.signal(); //give slot back

    //inform customer of completed transaction
print("Teller", id, "Customer", cust, "inform cust transaction is complete");
        txnDone[id].signal();

    //wait for customer to depart
    custLeft[id].wait();
        print("Teller", id, "Customer", cust, "customer has left, finished serving");
}
}

//customer thread
void customer (int id) {
    //randomlyu pick transaction and wait
    //wait for bank to open, 2 in at a time
    //get custome rin line aand choose the teller free for the longest
}