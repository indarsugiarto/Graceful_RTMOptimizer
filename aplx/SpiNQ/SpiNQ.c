#include "SpiNQ.h"

/* TODO-1: how to adjust Q-Learning parameters:
 * 1. adjust learning rate adaptively
 * 2. adjust exploration ration adaptively
 *
 * TODO-2: when to change from learning phase to applying phase?
 * */

/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------- runQ() is the main function of Q-learning ----------*/

void initQ()
{
	isRunning = FALSE;
	isLearning = TRUE;
	alphaVal = 0.1;	// learning rage
	gammaVal = 0.75;	// discount factor
	betaVal = 0.5;		// exploration ratio

	io_printf(IO_STD, "alpha=%k, gamma=%k, beta=%k\n", alphaVal, gammaVal, betaVal);

	// create a list of frequency
	for(uchar i=0; i<N_FREQ_ITEM; i++)
		fList[i] = 100 + i*10;

	// let's randomly initialize currentState
	//uchar s = sark_rand() % N_STATES;
	//uchar a = sark_rand() % N_ACTION;

	// set the current state based on the current frequency
	currentState.state = getStateFromFreq(readSpinFreqVal());
}

uchar getStateFromFreq(uint freq)
{
	uchar result;
	for(uchar i=0; i<N_FREQ_ITEM; i++)
		if (freq==fList[i]) {
			result = i;
			break;
		}
	return result;
}

void runQ()
{
	// stage1: collect measurement and compute reward
	//		   it operates continuously, independent of learning state
	collectMeasurement();
	// TODO: at some point cpuIdleCntr need to be reset (inside computeAvgCPUidle())
	computeAvgCPUidle();
	computeCPUperf();

	// the reward is based on measurement and performance above
	computeReward();

	// optionally, send the measurement/reward to host
	sendMReport();

	// stage2: update Q-matrix
	//         it runs only if isRunning is true, which can be triggered
	//         by an "SDP-command" from host-PC
	if(isRunning == TRUE) {
		//if(1) {
		if(isLearning==TRUE) {
			io_printf(IO_STD, "Q-learning is in learning phase...\n");
			//updateQ();
		}
		else {
			io_printf(IO_STD, "Q-learning is not in learning phase...\n");
		}
		//TODO: when to change from learning phase to applying phase?
		updateQ();


		// optionally, send the Q-matrix to host
		sendQReport();
		// change the frequency based on the current state
		//changeFreq(getFreqFromState(currentState));
	}
	else {

	}
}

/*_______________________________________________________________*/

/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*--------------------- Operation Elements ----------------------*/

// find the maximum future Q-value according to the selected action
int maxFutureQVal(st_t currSt)
{
	int maxQ;
	uchar i;
	st_t dummyState;
	uchar possibleNextState[N_ACTION];
	for(i=0; i<N_ACTION; i++) {
		dummyState.state = currSt.state;
		dummyState.action = (act_t)i;
		possibleNextState[i] = getNexStateFromAction(dummyState);
	}
	int possibleQ[N_ACTION];
	for(i=0; i<N_ACTION; i++) {
		possibleQ[i] = Q[possibleNextState[i]][possibleNextState[i]];
	}
	maxQ = possibleQ[0];
	for(i=1; i<N_ACTION; i++) {
		if(possibleQ[i] > maxQ) {
			maxQ = possibleQ[i];
		}
	}
	return maxQ;
}

uchar getNexStateFromAction(st_t currSt)
{
	uchar result;
	// if in the boundary, than remain there
	if((currSt.state==0 && currSt.action==DOWN) ||
	   (currSt.state==(N_STATES-1) && currSt.action==UP)) {
		result = currSt.state;
	}
	else {
		switch(currSt.action) {
		case DOWN:
			result = currSt.state - 1; break;
		case STAY:
			result = currSt.state; break;
		case UP:
			result = currSt.state + 1; break;
		}
	}
	return result;
}

uint getFreqFromState(st_t s)
{
	return fList[s.state];
}

act_t getActionWithMaxQ(st_t currSt)
{
	int Qval[N_ACTION];
	uchar i;
	act_t a;
	st_t dummyState;
	uchar sIdx;
	int maxQ;
	dummyState.state = currSt.state;
	for(i=0; i<N_ACTION; i++) {
		if(currSt.state==0 || currSt.state==(N_STATES-1))
			a = STAY;
		else
			a = (act_t)i;
		dummyState.action = a;
		sIdx = getNexStateFromAction(dummyState);
		Qval[i] = Q[sIdx][sIdx];
	}
	uchar maxAction = 0;
	maxQ = Qval[maxAction];
	for(i=1; i<N_ACTION; i++) {
		if(Qval[i] > maxQ) {
			maxQ = Qval[i];
			maxAction = i;
		}
	}
	return (act_t)maxAction;
}

void updateQ()
{
	// for debugging, fills Q with timer tick:
	/*
	for(int i=0; i<N_STATES; i++) {
		for(int j=0; j<N_STATES; j++) {
			Q[i][j] = tCntr;
		}
	}
	*/
	tCntr++;

	io_printf(IO_STD, "Updating Q-values...\n");

	// stage-1: determine if we're exploring or exploiting
	uint rv = sark_rand() % 65535;	// the max value of REAL is 65535
	REAL ex = (REAL)rv / (REAL)65535;
	//io_printf(IO_STD, "ex=%k, betaVal=%k\n", ex, betaVal);
	if(ex > betaVal) {
		isExploring = TRUE;
		io_printf(IO_STD, "Exploring...\n");
	}
	else {
		isExploring = FALSE;
		io_printf(IO_STD, "Exploiting...\n");
	}

	// If exploration is active, then next action selection is based on
	// a random probability value. Otherwise, it select the best action
	// that leads to the maximum Q_value/reward.

	// Formula: Q'(s,a) = Q(s,a) + a*{r + g*[max_Q(s',a)] - Q(s,a)}
	// requirement: reward

	// stage-2: select an action, either randomly of with best known value
	if(isExploring==TRUE) {
		currentState.action = sark_rand() % N_ACTION;	// DOWN, STAY of UP
	}
	else {
		// in the beginning, if exploitation is chosen, then
		// this algorithm very likely will choose act_t::DOWN, because Q is 0
		currentState.action = getActionWithMaxQ(currentState);
	}

	// stage-3: get the maximum future-Q based on the selected action
	int maxQ = maxFutureQVal(currentState);

	// stage-4: update Q table
	int currentQ = Q[currentState.state][currentState.state];
	REAL deltaQ = alphaVal * ((REAL)currentRewardVal + gammaVal * (REAL)maxQ -
				  (REAL)currentQ);

	// with reward only +1 and -1, deltaQ will be less than 1,
	// hence, nextQ will always 0. Thus, we need to multiply with something
	//int nextQ = currentQ + (int)(deltaQ*1000);
	int nextQ = currentQ + (int)deltaQ;

	io_printf(IO_STD, "a=%d, maxFutureQ=%d, deltaQ=%k, nextQ=%d\n",
			  currentState.action, maxQ, deltaQ, nextQ);


	// then put the nextQ to the Qtable
	Q[currentState.state][currentState.state] = nextQ;

	// stage-5: go to the next state with the selected action
	currentState.state = getNexStateFromAction(currentState);
}

/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*----------------------- Get Measurements ----------------------*/
void collectMeasurement()
{
	// read frequency
	currentFreq = readSpinFreqVal();

	// read temperatures
	readTemp();

	// read average CPU load
	// computeAvgCPUidle();

	// optional: compute real temperature
	currentTempReal = getRealTemp();
}

void computeReward()
{
	// TODO: how to compute reward value?
	// currentRewardVal = (avgCPUidle/35) -  tempVal[2];
	currentRewardVal = (int)(100.00 - avgCPUload - currentTempReal);
	//if(currentTempReal < 41) currentRewardVal=1; else currentRewardVal=-1;
}

void computeCPUperf()
{
	// TODO: how? It is app-related function!
}

/*_______________________________________________________________*/


/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*-------------------- Reporting / Debugging---------------------*/

// use genericMsg for measurement reporting
/* format:
 *		cmd_rc = CPU performance
 *		seq = clock freqency
 *		arg1 = tempVal[0] -> just one, since tempVal[0] similar to tempVal[1]
 *		arg2 = tempVal[2] -> this is the most important temperature
 *		arg3 = currentTempReal
 *		data[0:3] = reward value
 *		data[4:7] = average cpu utilization
 *		data[8:11] = phycpu_0_idle_cntr
 *		data[12:15] = phycpu_1_idle_cntr
 *		... etc
 * */
void sendMReport()
{
	float t = (float)currentTempReal;
	genericMsg.cmd_rc = CPUperf;
	genericMsg.seq = currentFreq;
	genericMsg.arg1 = tempVal[0];
	genericMsg.arg2 = tempVal[2];
	sark_mem_cpy((void *)&genericMsg.arg3, (void *)&t, sizeof(uint));
	sark_mem_cpy((void *)genericMsg.data, (void *)&currentRewardVal, sizeof(uint));
	sark_mem_cpy((void *)genericMsg.data + sizeof(uint),
				 (void *)&avgCPUidle, sizeof(uint));
	for(uint i=0; i<18; i++)
		sark_mem_cpy((void *)genericMsg.data + ((2+i)*sizeof(uint)),
					 (void *)&cpuIdleCntr[i], sizeof(uint));
	spin1_send_sdp_msg(&genericMsg, SDP_TIMEOUT);
}

// The mechanism for sending Q-values: using seq as row counter
void sendQReport()
{
	// void * should be similar to char *
	void *ptr = (void *)Q;
	for(ushort row=0; row<N_STATES; row++) {
		reportMsg.seq = row;
		//ptr += row*N_STATES*sizeof(uint);
		sark_mem_cpy((void *)reportMsg.data, ptr, N_STATES*sizeof(uint));
		ptr += N_STATES*sizeof(uint);
		spin1_send_sdp_msg(&reportMsg, SDP_TIMEOUT);
		sark_delay_us(250);
	}
}

/* Since cpuIdleCntr work with physical core, host might
 * want to know, the mapping from physical to virtual core.
 * To do that, host needs to send SDP_CMD_SEND_CPU_MAP
 * */
void sendCPUmapReport()
{

}
