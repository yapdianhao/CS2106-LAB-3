/*************************************
* Lab 3 Exercise 4
* Name: Yap Dian Hao
* Student Id: A0184679H
* Lab Group: B13
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.

int start = -1;
extern int num_of_cars;
extern int num_of_segments;
sem_t* segment_mutexes;
sem_t roundabout_mutex;

void initialise()
{
    //TODO: Your code here
    sem_init(&roundabout_mutex, 0, num_of_segments - 1);
    segment_mutexes = malloc(sizeof(sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segment_mutexes[i], 0, 1);
    }
}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&segment_mutexes[i]);
    }
    sem_destroy(&roundabout_mutex);
    free(segment_mutexes);
}

void* car(void* car)
{
    //TODO: Your code here, see the general steps below

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    //
    car_struct *car_ptr = (car_struct*) car;

    sem_wait(&roundabout_mutex);
    sem_wait(&segment_mutexes[car_ptr->entry_seg]);
    enter_roundabout(car_ptr);

    while (car_ptr->current_seg != car_ptr->exit_seg) {
       int prev = car_ptr->current_seg;
       int next = NEXT(car_ptr->current_seg, num_of_segments);
       //int curr = PREV(next, num_of_segments);
       sem_wait(&segment_mutexes[next]);
       move_to_next_segment(car_ptr);
       sem_post(&segment_mutexes[prev]);
    }
    exit_roundabout(car_ptr);
    sem_post(&roundabout_mutex);
    sem_post(&segment_mutexes[car_ptr->exit_seg]);
    //sem_post(&roundabout_mutex);

    pthread_exit(NULL);
}
