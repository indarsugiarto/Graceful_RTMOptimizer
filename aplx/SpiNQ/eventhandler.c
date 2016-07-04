#include "SpiNQ.h"

extern uint cpuIdleCntr[18];						// for all cpus

void hDMADone(uint tid, uint tag)
{

}

// let's use timer-1 for anything else
void hTimer1(uint tick, uint Unused)
{
	//Note: the timer-1 tick's rate is affected by the frequency !!!

	io_printf(IO_BUF, "perf=%d, avg=%d, load=%2.2k, f=%d, t0=%d, t2=%d(%2.2k), rwd=%d\n",
			  CPUperf, avgCPUidle, (REAL)avgCPUload, currentFreq, tempVal[0], tempVal[2],
			  currentTempReal, currentRewardVal);
}


// timer-2 is more consistent than timer-1 because it adapts
// the frequency changes!!! So, let's use timer-2 to do the Q-learning stuffs:
// 1. collect measurement (and send to host via IPTAG_GENERIC)
// 2. compute reward
// 3. update Q-values
void hTimer2(uint tick, uint Unused)
{
	runQ();
}

void hSDP(uint mBox, uint port)
{
	sdp_msg_t *msg = (sdp_msg_t *)mBox;
	if(port == SDP_CMD_PORT) {
		if(msg->cmd_rc == SDP_CMD_SET_FREQ) {
			// the new freq should be carried in seq
			changeFreq(msg->seq);
			currentFreq = readSpinFreqVal();
		}
		else if(msg->cmd_rc == SDP_CMD_GOTO_STATE) {

		}
		else if(msg->cmd_rc == SDP_CMD_SEND_CPU_MAP) {

		}
		else if(msg->cmd_rc == SDP_CMD_REPORT_PLL) {
			if(msg->seq == 0)
				readPLL(IO_BUF);
			else
				readPLL(IO_STD);
		}
		else if(msg->cmd_rc == SDP_CMD_RUNQ) {
			io_printf(IO_STD, "Asking to run Q-learning...\n");
			isRunning = TRUE;
		}
		else if(msg->cmd_rc == SDP_CMD_STOPQ) {
			io_printf(IO_STD, "Asking to postpone Q-learning...\n");
			isRunning = FALSE;
		}
	}
	else if(port == SDP_CONFIG_PORT) {

	}

	sark_msg_free(msg);
}

void hMCPL(uint key, uint payload)
{

}

