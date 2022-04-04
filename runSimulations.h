#ifndef __RUNSIMULATIONS_H
#define __RUNSIMULATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "queue.h"

/* function prototypes */
float* runOneSimulation(int seed, int leftPeriod, int rightPeriod, float leftCarArrivalRate, float rightCarArrivalRate);

#endif