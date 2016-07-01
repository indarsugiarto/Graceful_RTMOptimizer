#include "SpiNQ.h"

/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------- runQ() is the main function of Q-learning ----------*/

void initQ()
{
	isRunning = FALSE;
	isLearning = TRUE;

	// let's randomly initialize currentState
	uchar x = sark_rand() % N_STATES;
	uchar y = sark_rand() % N_STATES;
}

void runQ()
{
	// stage1: collect measurement and compute reward
	//		   it operates continuously, independent of learning state
	collectMeasurement();
	computeCPUperf();
	computeReward();
	// optionally, send the measurement/reward to host
	sendMReport();

	// stage2: update Q-matrix
	//         it runs only if isRunning is true, which can be triggered
	//         by an "SDP-command" from host-PC
	if(isRunning == TRUE) {
		updateQ();
		// optionally, send the Q-matrix to host
		sendQReport();
	}
}

/*_______________________________________________________________*/

/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*--------------------- Operation Elements ----------------------*/

// find the maximum future Q-value at (xQ, yQ)
void maxQVal(short maxVal[N_ACTION], uchar xQ, uchar yQ)
{

}

short getReward()
{

}

void updateQ()
{

}

void collectMeasurement()
{
	// read frequency
	currentFreq = readSpinFreqVal();

	// read temperatures
	readTemp();

	// optional: compute real temperature

}

void computeReward()
{
	// TODO: how to compute reward value?
	currentRewardVal = tempVal[2];
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
	genericMsg.cmd_rc = CPUperf;
	genericMsg.seq = currentFreq;
	genericMsg.arg1 = tempVal[0];
	genericMsg.arg2 = tempVal[2];
	genericMsg.arg3 = currentTempReal;
	sark_mem_cpy((void *)genericMsg.data, (void *)&currentRewardVal, sizeof(uint));
	sark_mem_cpy((void *)genericMsg.data + sizeof(uint),
				 (void *)&avgCPUidle, sizeof(uint));
	for(uint i=0; i<18; i++)
		sark_mem_cpy((void *)genericMsg.data + ((2+i)*sizeof(uint)),
					 (void *)&cpuIdleCntr[i], sizeof(uint));
	spin1_send_sdp_msg(&genericMsg, SDP_TIMEOUT);
}

void sendQReport()
{

}

/* Since cpuIdleCntr work with physical core, host might
 * want to know, the mapping from physical to virtual core.
 * To do that, host needs to send SDP_CMD_SEND_CPU_MAP
 * */
void sendCPUmapReport()
{

}
