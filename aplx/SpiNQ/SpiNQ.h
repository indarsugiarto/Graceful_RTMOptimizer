/* Contains all definition and variable declarations that are related
 * with Q-Learning.
 * */
#ifndef SPINQ_H
#define SPINQ_H

#define MAJOR_VERSION	0
#define MINOR_VERSION	1

#define USE_SPIN3
//#define USE_SPIN5

#include "stdspinapi.h"

#define N_FREQ_ITEM		25
#define	N_STATES		N_FREQ_ITEM

// Q-table contains basically integer values
typedef struct StAct
{
	int Qval;
	int action[N_STATES];	// it can go anywhere, including stay in the same state?
} StAct_t;

StAct_t Q[N_STATES][N_STATES];


// forward declarations
int maxQVal(StAct_t s);		// get the maximum Q-value at state-s

#endif
