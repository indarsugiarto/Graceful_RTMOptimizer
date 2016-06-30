#include "SpiNQ.h"

void hDMADone(uint tid, uint tag)
{

}

void hTimer1(uint tick, uint Unused)
{
	// io_printf(IO_STD, "Tick1-%d\n", tick);
	if(tick==0)
		readPLL(IO_STD);
}

// let's use timer-2 to collect measurement and send to host via
// IPTAG_GENERIC
void hTimer2(uint tick, uint Unused)
{
	//io_printf(IO_STD, "Tick2-%d\n", tick);
	currentFreq = readSpinFreqVal();
	readTemp();	// the result will be stored in tempVal[]
	io_printf(IO_BUF, "Freq = %u, T1 = %d, T2 = %d, T3 = %d\n",
			  currentFreq, tempVal[0], tempVal[1], tempVal[2]);
}

void hSDP(uint mBox, uint port)
{

}

void hMCPL(uint key, uint payload)
{

}

