#include "SpiNQ.h"

void hDMADone(uint tid, uint tag)
{

}

// let's use timer-1 to do the Q-learning stuffs:
// 1. collect measurement (and send to host via IPTAG_GENERIC)
// 2. compute reward
// 3. update Q-values
void hTimer1(uint tick, uint Unused)
{
	// io_printf(IO_STD, "Tick1-%d\n", tick);
	if(tick==0)
		readPLL(IO_STD);
	else {
		runQ();
	}
}

// let's use timer-2 to measure CPU performance
void hTimer2(uint tick, uint Unused)
{
	//io_printf(IO_STD, "Tick2-%d\n", tick);
	currentFreq = readSpinFreqVal();
	readTemp();	// the result will be stored in tempVal[]
	io_printf(IO_BUF, "Freq = %u, T1 = %d, T2 = %d, T3 = %d\n",
			  currentFreq, tempVal[0], tempVal[1], tempVal[2]);

	// TODO: at some point cpuIdleCntr need to be reset
}

void hSDP(uint mBox, uint port)
{
	sdp_msg_t *msg = (sdp_msg_t *)mBox;
	if(port == SDP_CMD_PORT) {
		if(msg->cmd_rc == SDP_CMD_SET_FREQ) {

		}
		else if(msg->cmd_rc == SDP_CMD_GOTO_STATE) {

		}
		else if(msg->cmd_rc == SDP_CMD_SEND_CPU_MAP) {

		}
	}
	else if(port == SDP_CONFIG_PORT) {

	}

	sark_msg_free(msg);
}

void hMCPL(uint key, uint payload)
{

}

