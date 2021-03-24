/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include <stdlib.h>
#include <semaphore.h>
#include "barrier.h"

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    barrier->sem = malloc(sizeof(sem_t));
    barrier->mutex = malloc(sizeof(sem_t));
    barrier->count = count;
    barrier->arrived = 0;
    sem_init(barrier->sem, 0, 0);
    sem_init(barrier->mutex, 0, 1);
}

void barrier_wait ( barrier_t *barrier ) 
{
    sem_wait(barrier->mutex);
    barrier->arrived++;
    sem_post(barrier->mutex);
    if (barrier->count == barrier->arrived) {
        sem_post(barrier->sem);
    }
    sem_wait(barrier->sem);
    sem_post(barrier->sem);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    sem_destroy(barrier->mutex);
    sem_destroy(barrier->sem);
}
