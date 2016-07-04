/* Contains all definition and variable declarations that are related
 * with Q-Learning.
 *
 * The list of possible frequencies is created in initQ().
 *
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
static int Q[N_STATES][N_STATES] = {0};
st_t currentState;
//st_t nextState;

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
volatile uchar isRunning;			// set this value to run or pause Q-learning
volatile uchar isLearning;			// set this value to learning or applying Q
uchar isExploring;			// set this value to explore rather than exploit
REAL  alphaVal;				// learning rate
REAL  gammaVal;				// discount factor
REAL  betaVal;					// exloration ratio

// the main function that runs Q-learning
void runQ();

// other forward declarations - will be in SpiNQ.c
// int maxQVal(StAct_t s);		// get the maximum Q-value at state-s
void initQ();
int maxFutureQVal(st_t currSt);					// will compute max Q value based on selected action a
void updateQ();

// misc functions - will be implemented in SpiNQ.c
void collectMeasurement();
void computeReward();
void computeCPUperf();
void sendMReport();
void sendQReport();
void sendCPUmapReport();
uchar getStateFromFreq(uint freq);		// this will use Freq list generated in initQ
uchar getNexStateFromAction(st_t currSt);	//
uint getFreqFromState(st_t s);
act_t getActionWithMaxQ(st_t currSt);		// for exploitation, get the action that leads to
											// maximum reward/Q-value

// helper functins - will be implemented in util.c
REAL getRealTemp();				// get a "Real" temperature in Celcius
void readPLL(char *stream);

#endif
