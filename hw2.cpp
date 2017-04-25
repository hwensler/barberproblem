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


void *VisitBarber(void *threadID){

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
            cout << " which is not between 0 and 25. \n";
        }
    }

    //create semaphores
    sem_t waitChairs;
    sem_t mutexBarber;

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


    //create threads
    for (int i = 0; i < customerCount; i++) {
        //thread creation = leaving for the barber shop
        cout << "Customer " << i;
        cout << "is leaving for the barber shop. ";
        errorCheck = pthread_create(&threads[i], &attribute, &VisitBarber, (void *) barber);

        //check that the threads were created
        if (errorCheck) {
            cout << "Error: unable to create thread " << errorCheck << endl;
            exit(-1);
        }
    }


    pthread_exit(NULL);
    //create a thread for each customer

    //put them in an array

}
