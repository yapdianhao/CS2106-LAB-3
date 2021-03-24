/*************************************
* Lab 3 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/************************************

You should use ex2 solution as the starting point.

Copy over the solution and modify as needed.
************************************/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->read_mutex), NULL);
  pthread_mutex_init(&(lock->next_mutex), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->next_mutex));
  pthread_mutex_lock(&(lock->mutex));
  pthread_mutex_unlock(&(lock->next_mutex));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->next_mutex));
  pthread_mutex_lock(&(lock->read_mutex));
  lock->reader_count++;
  if (lock->reader_count == 1) pthread_mutex_lock(&(lock->mutex));
  pthread_mutex_unlock(&(lock->next_mutex));
  pthread_mutex_unlock(&(lock->read_mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->read_mutex));
  lock->reader_count--;
  if (lock->reader_count == 0) pthread_mutex_unlock(&(lock->mutex));
  pthread_mutex_unlock(&(lock->read_mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
}
