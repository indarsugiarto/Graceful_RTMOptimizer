#include "SpiNQ.h"

// find the maximum future Q-value at (xQ, yQ)
void maxQVal(short maxVal[N_ACTION], uchar xQ, uchar yQ)
{

}

short getReward()
{

}

void updateQ(uint state, uint action)
{

}

void updateReport(uint tick, uint arg1)
{
	/*
	// get the idle counters
	cpuIdleCntr[myPhysicalCore] = myOwnIdleCntr;	// replace the "always zero counter" that runs on myself
	sark_mem_cpy((void *)report.data, (void *)cpuIdleCntr, 18*sizeof(uint));
	// clear the idle counters and play with the LEDs
	for(_idleCntr=0; _idleCntr<18; _idleCntr++)
		cpuIdleCntr[_idleCntr] = 0;
	myOwnIdleCntr = 0;

	report.cmd_rc = SPINN_SEND_REPORT;
	report.length = szReport;
	if(bRptStreamEnable)
		spin1_send_sdp_msg(&report, DEF_TIMEOUT);
	//io_printf(IO_BUF, "Sending report using iptag-%u on port-%u...\n", report.tag, DEF_REPORT_PORT);
	*/
}

void collectMeasurement()
{
	// read frequency
	currentFreq = readSpinFreqVal();

	// read temperatures
	readTemp();
}

uint computeReward()
{
	rewardVal = tempVal[2];
}
