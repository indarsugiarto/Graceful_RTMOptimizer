/* Contains all definition and variable declarations that are related
 * with Q-Learning.
 * */
#ifndef SPINQ_H
#define SPINQ_H

#define MAJOR_VERSION	0
#define MINOR_VERSION	1

#define USE_SPIN3
//#define USE_SPIN5

#include <stdfix.h>
#include "stdspinapi.h"
#include "profiler.h"

// general/global parameters
#define REAL						accum
#define REAL_CONST(x)				(x##k)
#define N_FREQ_ITEM					16
#define	N_STATES					N_FREQ_ITEM
#define N_ACTION					3

typedef enum {DOWN, STAY, UP} act_t;

typedef struct st
{
    uchar state;    // from 0 to N_STATES-1
    act_t action;   //
} st_t;

// core Q-learning components
static uint Q[N_STATES][N_STATES] = {0};
uchar currentState[2];

// state parameters/measurements
ushort fList[N_FREQ_ITEM];
uint CPUperf;				// CPU performance, how to measure it?
uint currentFreq;
uint currentTempInt;
REAL currentTempReal;
int currentRewardVal;
// the following will be collected from profiler:
// uint tempVal[3];
// uint cpuIdleCntr[18];
// uint avgCPUidle;

// dummy variable:
uint tCntr;

// these will control how Q-learning operates:
uchar isRunning;			// set this value to run or pause Q-learning
uchar isLearning;			// set this value to explore or exploit

// the main function that runs Q-learning
void runQ();

// other forward declarations - will be in SpiNQ.c
// int maxQVal(StAct_t s);		// get the maximum Q-value at state-s
void initQ();
void maxQVal(short maxVal[N_ACTION], uchar xQ, uchar yQ);
void updateQ();
short getReward();

// misc functions - will be implemented in SpiNQ.c
void collectMeasurement();
void computeReward();
void computeCPUperf();
void sendMReport();
void sendQReport();
void sendCPUmapReport();

// helper functins - will be implemented in util.c
REAL getRealTemp();				// get a "Real" temperature in Celcius
void readPLL(char *stream);

#endif
