/**
 * Heather Wensler
 * CPSC 5942
 * Homework 2
 * hw2.cpp
 */

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <semaphore.h>
#include <mutex>

using namespace std;


/**
 *The barber shop has one barber, one barber chair, and n chairs for
 * waiting customers, if any, to sit on. If there are no customers present,
 * the barber sits down in the barber chair and falls asleep.
 * When a customer arrives and the barber is sleeping, s/he has to wake
 * up the sleeping barber. If additional customers arrive while the
 * barber is cutting a customer’s hair, they either sit down (if
 * there are empty chairs) or wait for the empty chairs (if all chairs
 * are full). The problem is to program the barber and the customers
 * without getting into race conditions.
 *
 * max customers = 25.
 */

/**
 * A customer
 * 1. enters.
 *
 * 2. if the barber is asleep, wake the barber.
 * 3. sit in the barber chair.
 * 4. get their hair cut.
 *
 * 5. if the barber chair is occupied by a customer
 * sit in a waiting chair. if there are no waiting chairs,
 * wait for a chair.
 *
 * Resources:
 * barber chair - 1 (use mutex lock)
 * waiting chairs - user input (use semaphore)
 */

//define max number of customers
#define MAX 25;

//initialize the waiting room
int waitingRoom[MAX];

//this semaphore is for the waiting room chairs.
//there should be as many resources as there are chairs
sem_t emptyChairs;

//there should be a semaphore for full chairs
//it should start at 0
sem_t fullChairs;


//this is for the cout function. there should only be one (mutex).
//it makes cout legible
pthread_mutex_t coutMutex = PTHREAD_MUTEX_INITIALIZER;

//create struct to pass to thread functions
struct thread_details{
    //the thread id
    int threadID;

    //the total number of threads
    int totalCustomers;
};

//create an array of the customers in line
int getHaircut(){
    int id = waitingRoom[25];
}

void *GoToWork(void *customer_info){

    struct thread_details *p = (struct thread_details*) customer_info;

    //at first the barber is always sleeping
    pthread_mutex_lock(&coutMutex);
    cout << "The barber is sleeping";
    pthread_mutex_unlock(&coutMutex);

    //for every customer, be in this loop
    for(int i = 0; i < p->totalCustomers; i++){
        //if there is a full chair
        int numFullChairs;
        sem_getvalue(&fullChairs, &numFullChairs);

        if(numFullChairs > 0){
            //cut their hair
            pthread_mutex_lock(&coutMutex);
            cout << "The barber is cutting hair";
            pthread_mutex_unlock(&coutMutex);

        }
        //else
        else{
            //go to sleep
            pthread_mutex_lock(&coutMutex);
            cout << "The barber is sleeping. ";
            pthread_mutex_unlock(&coutMutex);

            //and wait for someone else to show up
            sem_wait(&fullChairs);

            //then cut their hair
            pthread_mutex_lock(&coutMutex);
            cout << "The barber is cutting hair. ";
            pthread_mutex_unlock(&coutMutex);

            //which frees up a chair
            sem_post(&emptyChairs);

        }

    }

}


void *VisitBarber(void *customer_info){
    //*p is a pointer to this thread_details
    struct thread_details *p = (struct thread_details*) customer_info;
    pthread_mutex_lock(&coutMutex);
    cout << "Customer " << p->threadID <<" arrived at the barber shop.\n ";
    pthread_mutex_unlock(&coutMutex);

    //get the current value of the semaphore
    int currentValue;
    sem_getvalue(&emptyChairs, &currentValue);

    //if you can, take a seat
    sem_wait(&emptyChairs);  //empty chairs decreases by one
    pthread_mutex_lock(&coutMutex);
    cout << "Customer " << p->threadID << " took a seat in the waiting room.\n";
    pthread_mutex_unlock(&coutMutex);
    sem_post(&fullChairs);

    //if you're the first person to arrive, wake the barber
    //put a mutex lock around this???????? since only one person can be waking up the barber
    if(currentValue == p->totalCustomers - 1){
        pthread_mutex_lock(&coutMutex);
        cout << "Customer " << p->threadID << " woke the barber.\n";
        pthread_mutex_unlock(&coutMutex);
    }

    //leave the barber shop
    pthread_mutex_lock(&coutMutex);
    cout << "Customer " << p->threadID << " is leaving the barber shop.\n";
    pthread_mutex_unlock(&coutMutex);

}



int main() {


    //introduce the program
    cout << "Welcome to the barber shop!\n";

    //declare customers - 26 so it enters the while loop to get input
    int customerCount = 26;

    //get the number of customers - must be between 0 and 25
    while (customerCount > 25 || customerCount <= 0) {
        cout << "How many customers are there?\n"
                "There must be between 0 and 25 customers.\n";
        cin >> customerCount;

        //if the customercount is still invalid
        if (customerCount > 25 || customerCount < 0) {
            cout << "Sorry, you entered ";
            cout << customerCount;
            cout << " which is not between 0 and 25.\n";
        }
    }

    //declare waiting chairs
    int numChairs;
    cout << "How many chairs are there in the waiting room? \n";
    cin >> numChairs;

    //this semaphor is for the empty waiting room chairs.
    // there should be as many resources as there are chairs
    sem_init(&emptyChairs, 0, numChairs);

    //this semaphor is for the full waiting room chairs.
    sem_init(&emptyChairs, 0, 0);

    //create thread ids
    pthread_t threads[customerCount];

    //create an int to determine if thread creation was successful
    int errorCheck;

    //create thread attributes
    pthread_attr_t attribute;

    //status of the thread
    void *status;

    //initialize and set thread joinable
    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

    //create an array of structs to pass to threads
    thread_details allThreads[customerCount];


    //create threads
    for (int i = 0; i < customerCount; i++) {
        //create struct to pass info to VisitBarber
        allThreads[i].totalCustomers = customerCount;
        allThreads[i].threadID = i;

        //create a pointer to be sent to the thread
        thread_details *tempThread;

        //assign pointer to the correct struct in allThreads
        tempThread = &allThreads[i];

        //thread creation = leaving for the barber shop
        pthread_mutex_lock(&coutMutex);
        cout << "Customer " << i << " is leaving for the barber shop.\n";
        pthread_mutex_unlock(&coutMutex);

        //create the thread
        errorCheck = pthread_create(&threads[i], &attribute, &VisitBarber, (void *)tempThread);

        //check that the threads were created
        if (errorCheck) {
            cout << "Error: unable to create thread " << errorCheck << endl;
            exit(-1);
        }
    }

    //free the attribute and wait for other threads
    pthread_attr_destroy(&attribute);

    //make an int for results
    int results;

    //join the threads
    for (int i = 0; i < customerCount; i++){
        //this waits for all the results to be completed
        results = pthread_join(threads[i], &status);

        if (results){
            cout << "Error:unable to join," << results << endl;
            exit(-1);
        }
        pthread_mutex_lock(&coutMutex);
        cout << "Main: completed thread id :" << i;
        cout << "  exiting with status :" << status << endl;
        pthread_mutex_unlock(&coutMutex);

    }

    cout << "Main: program exiting." << endl;

    cout.flush();
    pthread_exit(NULL);

}
