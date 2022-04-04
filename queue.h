#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdlib.h>
#include <stdio.h>

/* structure definitions */
struct waitingCar {
  unsigned int timeArrived;
  struct waitingCar *next;
};
typedef struct waitingCar CAR;

struct queue{
  int size;
  CAR *front;
  CAR *rear;
};
typedef struct queue QUEUE;

/* function prototypes */
void newCar(QUEUE *q, unsigned int time);
int carThrough(QUEUE *q);

#endif