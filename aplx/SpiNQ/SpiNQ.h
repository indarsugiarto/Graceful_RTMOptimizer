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
#define N_ACTION        3

// Q-table contains basically integer values
/*
typedef struct StAct
{
	int Qval;
    //int action[N_STATES];
    int action[3];
} StAct_t;

StAct_t Q[N_STATES][N_STATES];
*/

typedef enum {DOWN, STAY, UP} act_t;

typedef struct st
{
    uchar state;    // from 0 to N_STATES-1
    act_t action;   //
} st_t;

static short Q[N_STATES][N_STATES] = {0};
uchar currentState[2];

// forward declarations
// int maxQVal(StAct_t s);		// get the maximum Q-value at state-s
void maxQVal(short maxVal[N_ACTION], uchar xQ, uchar yQ);
void updateQ(uint state, uint action);
short getReward();

#endif
