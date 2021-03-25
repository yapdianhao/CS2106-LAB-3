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
// one mutex to lock each segment to prevent cars from going into same segment at same time
sem_t* segment_mutexes;
// one mutex to lock entire roundabout so that one space is always available
// one car is able to move to prevent deadlock
sem_t roundabout_mutex;

void initialise()
{
    sem_init(&roundabout_mutex, 0, num_of_segments - 1);
    segment_mutexes = malloc(sizeof(sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&segment_mutexes[i], 0, 1);
    }
}

void cleanup()
{
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&segment_mutexes[i]);
    }
    sem_destroy(&roundabout_mutex);
    free(segment_mutexes);
}

void* car(void* car)
{

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    //
    car_struct *car_ptr = (car_struct*) car;

    // consumes one charge of roundabout mutex when one car enters roundabout
    sem_wait(&roundabout_mutex);
    // lock the current segment as the car enters
    sem_wait(&segment_mutexes[car_ptr->entry_seg]);
    enter_roundabout(car_ptr);  
    // cannot call signal(segment_mutex[start_segment]) here!!
    // if signal, lock for this segment is released, other car can other. 2 cars in one segment!
    
    while (car_ptr->current_seg != car_ptr->exit_seg) {
       int prev = car_ptr->current_seg;
       int next = NEXT(car_ptr->current_seg, num_of_segments);
       //int curr = PREV(next, num_of_segments);

       // lock the next segment that is going to enter
       sem_wait(&segment_mutexes[next]);
       move_to_next_segment(car_ptr);
       // after leaving previous segment, release it to other cars
       sem_post(&segment_mutexes[prev]);
    }

    exit_roundabout(car_ptr);
    // one car leaves the roundabout, add one charge to roundabout_mutex
    sem_post(&roundabout_mutex);
    // car leaves the last segment, release to other cars
    sem_post(&segment_mutexes[car_ptr->exit_seg]);

    pthread_exit(NULL);
}
