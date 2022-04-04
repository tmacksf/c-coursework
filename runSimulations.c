#include "runSimulations.h"

/* functions */
float* runOneSimulation(int seed, int leftPeriod, int rightPeriod, float leftCarArrivalRate, float rightCarArrivalRate) {
  int iterationCount = 0;
  int both_queue_size = 0;

  /* random number generation */
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  gsl_rng_set(r, time(0)+seed);

  /* creating queues */
  QUEUE *rightQueue;
  QUEUE *leftQueue;
  if ((rightQueue = (QUEUE *)malloc(sizeof(QUEUE)) ) == NULL ) {
    printf("Not enough memory");
    exit(-1);
  }
  if ((leftQueue = (QUEUE *)malloc(sizeof(QUEUE)) ) == NULL ) {
    printf("Not enough memory");
    exit(-1);
  }
  /* checks to make sure that they are allocated memory space */

  /* sets the values to NULL */
  rightQueue->front = NULL;
  rightQueue->rear = NULL;
  rightQueue->size = 0;

  leftQueue->front = NULL;
  leftQueue->rear = NULL;
  leftQueue->size = 0;

  /* current green light where the left is 0 and right is 1 */
  int current_light = 0;
  int current_light_period = leftPeriod;
  int iterations_since_light_change = 0;

  /* information gathering for the output */
  /* left */
  int leftPassedThrough = 0;
  float leftExtraTime = 0;
  float leftLongestWaiting = 0;
  int totalLeftWaitingTimes = 0;
  /* right */
  int rightPassedThrough = 0;
  float rightExtraTime = 0;
  float rightLongestWaiting = 0;
  int totalRightWaitingTimes = 0;

  while (iterationCount < 500 || both_queue_size > 0) {
    iterationCount += 1;
    /* light changing section */

    if (iterations_since_light_change == current_light_period){
      if (current_light == 0) {
        /* change which light is green */
        current_light = 1;
        iterations_since_light_change = 0;
        current_light_period = rightPeriod;
        /* changes the current light period */
      }
      else {
        /* change which light is green */
        current_light = 0;
        iterations_since_light_change = 0;
        current_light_period = leftPeriod;
        /* changes the current light period */
      }
    }
      /* car action section */
    else {
      iterations_since_light_change += 1;
      /* generates the random numbers for car arrival */
      float randomCarLeft = gsl_ran_flat(r, 0, 1);
      float randomCarRight = gsl_ran_flat(r, 0, 1);

      /* adds a car if the random numbers are lower than the rate */
      if (randomCarLeft < leftCarArrivalRate && iterationCount < 500){
        newCar(leftQueue, iterationCount);
      }
      if (randomCarRight < rightCarArrivalRate && iterationCount < 500) {
        newCar(rightQueue, iterationCount);
      }

      /* gets the entry time of the car that passed through the queue */

      int entryTime;
      if (current_light == 0) {
        /* sends car through and gets the entry time of that car */
        entryTime = carThrough(leftQueue);
        if (entryTime >= 0){
          /* if a car went through, the entry time is added to the total time */
          int totalTime = iterationCount - entryTime;
          totalLeftWaitingTimes += totalTime;
          leftPassedThrough += 1;
          /* checks if the waiting time was the longest */
          if ((float)totalTime > leftLongestWaiting){
            leftLongestWaiting = (float)totalTime;
          }
        }
      }
      else {
        entryTime = carThrough(rightQueue);
        if (entryTime >= 0) {
          int totalTime = iterationCount - entryTime;
          totalRightWaitingTimes += totalTime;
          rightPassedThrough += 1;
          if ((float)totalTime > rightLongestWaiting){
            rightLongestWaiting = (float)totalTime;
          }
        }
      }
    }

    if (iterationCount > 500) {
      if (rightQueue->size > 0) {
        rightExtraTime += 1;
      }
      if (leftQueue->size > 0) {
        leftExtraTime += 1;
      }
    }
    both_queue_size = rightQueue->size + leftQueue->size;
  }

  float leftAverageWaitingTime;
  float rightAverageWaitingTime;

  leftAverageWaitingTime = ((float)totalLeftWaitingTimes)/(float)leftPassedThrough;

  rightAverageWaitingTime = ((float)totalRightWaitingTimes)/(float)rightPassedThrough;

  float *outValues;
  if ( (outValues = (float *)malloc(sizeof(float)*8)) == NULL ) {
    printf("Not enough memory");
    exit(-1);
  }

  outValues[0] = (float)leftPassedThrough;
  outValues[1] = leftLongestWaiting;
  outValues[2] = leftAverageWaitingTime;
  outValues[3] = leftExtraTime;
  outValues[4] = (float)rightPassedThrough;
  outValues[5] = rightLongestWaiting;
  outValues[6] = rightAverageWaitingTime;
  outValues[7] = rightExtraTime;

  /* printf("Left through %f\n", outValues[0]);
  printf("Left longest %f\n", leftLongestWaiting);
  printf("Left average %f\n", leftAverageWaitingTime);
  printf("Left extra %f\n", leftExtraTime);
  printf("Right through %d\n", rightPassedThrough);
  printf("Right extra %f\n", rightExtraTime);
  printf("Right average %f\n", rightAverageWaitingTime );
  printf("Right longest %f\n", rightLongestWaiting); */

  free(rightQueue);
  free(leftQueue);
  return outValues;
}

/* main function */

int main(){
  int leftPeriod;
  int rightPeriod;
  float leftCarArrivalRate;
  float rightCarArrivalRate;

  printf("Please enter the left light period as an integer: ");
  int validator = 0;
  while (validator == 0){
    if (scanf("%d", &leftPeriod) != 1) {
      printf("Failed, please try again.\n");
    }
    else {
      printf("Input valid.\n");
      validator = 1;
    }
  }
  validator = 0;
  printf("Please enter the right light period as an integer: ");
  while (validator == 0){
    if (scanf("%d", &rightPeriod) != 1) {
      printf("Failed, please try again.\n");
    }
    else {
      printf("Input valid.\n");
      validator = 1;
    }
  }

  validator = 0;
  printf("Please enter the left car arrival rate in the form of a float: ");
  while (validator == 0){
    if (scanf("%f", &leftCarArrivalRate) != 1) {
      printf("Failed, please try again.\n");
    }
    else {
      if (0 <= leftCarArrivalRate && leftCarArrivalRate <= 1) {
        validator = 1;
      } else {
        printf("Invalid input, please enter again.\n");
      }
    }
  }
  validator = 0;
  printf("Please enter the right car arrival rate in the form of a float: ");
  while (validator == 0){
    if (scanf("%f", &rightCarArrivalRate) != 1) {
      printf("Failed, please try again.\n");
    }
    else {
      if (0 <= rightCarArrivalRate && rightCarArrivalRate <= 1) {
        validator = 1;
      } else {
        printf("Invalid input, please enter again.\n");
      }
    }
  }

  printf("\nParameter Values:\n");
  printf("    From left:\n");
  printf("        Light period: %d\n",leftPeriod);
  printf("        Arrival rate %f\n",leftCarArrivalRate);
  printf("    From right:\n");
  printf("        Light period: %d\n",rightPeriod);
  printf("        Arrival rate: %f\n",rightCarArrivalRate);


  float leftPassedThrough = 0;
  float leftLongestWaiting = 0;
  float leftAverageTime = 0;
  float leftExtraTime = 0;

  float rightPassedThrough = 0;
  float rightLongestWaiting = 0;
  float rightAverageTime = 0;
  float rightExtraTime = 0;

  int i;
  for (i = 0; i < 100; i++){
    float *results;
    /* printf("Run: %d\n", i); */
    results = runOneSimulation(i, leftPeriod, rightPeriod, leftCarArrivalRate, rightCarArrivalRate);
    leftPassedThrough += results[0];
    leftLongestWaiting += results[1];
    leftAverageTime += results[2];
    leftExtraTime += results[3];

    rightPassedThrough += results[4];
    rightLongestWaiting += results[5];
    rightAverageTime += results[6];
    rightExtraTime += results[7];
  }

  float leftAveragePassedThrough = leftPassedThrough/100;
  float leftAverageOfLongest = leftLongestWaiting/100;
  float leftAverageOfAverage = leftAverageTime/100;
  float leftAverageExtraTime = leftExtraTime/100;

  float rightAveragePassedThrough = rightPassedThrough/100;
  float rightAverageOfLongest = rightLongestWaiting/100;
  float rightAverageOfAverage = rightAverageTime/100;
  float rightAverageExtraTime = rightExtraTime/100;

  printf("Results (Averaged over 100 runs):\n");
  printf("    From left:\n");
  printf("        Number of vehicles: %f\n", leftAveragePassedThrough);
  printf("        Maximum waiting time: %f\n", leftAverageOfLongest);
  printf("        Average waiting time: %f\n", leftAverageOfAverage);
  printf("        Clearance time: %f\n", leftAverageExtraTime);
  printf("    From right:\n");
  printf("        Number of vehicles: %f\n", rightAveragePassedThrough);
  printf("        Maximum waiting time: %f\n", rightAverageOfLongest);
  printf("        Average waiting time: %f\n", rightAverageOfAverage);
  printf("        Clearance time: %f\n", rightAverageExtraTime);

  return 0;
}