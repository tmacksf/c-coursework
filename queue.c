#include "queue.h"

void newCar(QUEUE *q, unsigned int time) {
  CAR *newCar;
  newCar = (CAR *)malloc(sizeof(CAR));
  if ( newCar == NULL ) {
    printf("Not enough memory");
    exit(-1);
  }
  /* setting values for the new car */
  newCar->timeArrived = time;
  newCar->next = NULL;
  q->size += 1;

  /* checking to see if the queue has anything */
  if (q->rear) {
    /* if it does, point to the new car that was just created as the next car */
    q->rear->next = newCar;
  }
  /* adding the new car to the end of the queue */
  q->rear = newCar;

  /* if there is no car at the front of the queue (empty queue) set this as the front */
  if (!q->front) {
    q->front = newCar;
  }
}

int carThrough(QUEUE *q) {
  if (!q->front) {
    /* if there are no cars the function returns -1 */
    return -1;
  }
  /* get the car at the front of the queue */
  CAR *car = q->front;
  /* makes the next car the front of the queue */
  q->front = q->front->next;
  unsigned int arrived = car->timeArrived;
  /* frees car */
  /* free(car); */
  q->size -= 1;
  return arrived;
}