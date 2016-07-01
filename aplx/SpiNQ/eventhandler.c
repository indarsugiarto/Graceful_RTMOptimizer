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
	runQ();
}


extern uint cpuIdleCntr[18];						// for all cpus
// let's use timer-2 to measure CPU performance
void hTimer2(uint tick, uint Unused)
{
	//io_printf(IO_STD, "Tick2-%d\n", tick);

	io_printf(IO_BUF, "perf = %d, avg = %d, f = %d, t0 = %d, t2 = %d, tReal = %k\n",
			  CPUperf, avgCPUidle, currentFreq, tempVal[0], tempVal[2], currentTempReal);

	// TODO: at some point cpuIdleCntr need to be reset (inside computeAvgCPUidle())
	computeAvgCPUidle();
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
			isRunning = TRUE;
		}
		else if(msg->cmd_rc == SDP_CMD_STOPQ) {
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

