#include "SpiNQ.h"

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------- Q-Learning related functionalities -----------------*/

extern void initProfiler();			// measurement-related
extern void terminateProfiler();
extern void initQ();				// Q-learning

/*_____________________________________________________________________*/



/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*------------------ Basic SpiNNaker functionalities ------------------*/

void initCallback()
{
    spin1_callback_on(DMA_TRANSFER_DONE, hDMADone, PRIORITY_DMA);
    spin1_callback_on(SDP_PACKET_RX, hSDP, PRIORITY_SDP);
    spin1_callback_on(MCPL_PACKET_RECEIVED, hMCPL, PRIORITY_MCPL);
}

void initIPTag()
{
	if(sv->p2p_addr==0) {
		sdp_msg_t iptag;
		iptag.flags = 0x07;	// no replay
		iptag.tag = 0;		// internal
		iptag.srce_addr = sv->p2p_addr;
		iptag.srce_port = 0xE0 + myCoreID;	// use port-7
		iptag.dest_addr = sv->p2p_addr;
		iptag.dest_port = 0;				// send to "root"
		iptag.cmd_rc = 26;
		// set the report tag
		iptag.arg1 = (1 << 16) + SDP_IPTAG_REPORT;
		iptag.arg2 = SDP_UDP_PORT_REPORT;
		iptag.arg3 = SDP_HOST_IP;
		iptag.length = sizeof(sdp_hdr_t) + sizeof(cmd_hdr_t);
		spin1_send_sdp_msg(&iptag, 10);
		// set the generic tag
		iptag.arg1 = (1 << 16) + SDP_IPTAG_GENERIC;
		iptag.arg2 = SDP_UDP_PORT_GENERIC;
		spin1_send_sdp_msg(&iptag, 10);
		// set the debug tag
		iptag.arg1 = (1 << 16) + SDP_IPTAG_ERR_DEBUG;
		iptag.arg2 = SDP_UDP_PORT_ERR_DEBUG;
		spin1_send_sdp_msg(&iptag, 10);
	}
}

void initSDP()
{
	// initialize reportMsg
	// let's use reportMsg to send Q-values
	reportMsg.flags = 0x07;
	reportMsg.tag = SDP_IPTAG_REPORT;
	reportMsg.srce_addr = sv->p2p_addr;
	reportMsg.srce_port = (SDP_CONFIG_PORT << 5) + myCoreID;
	reportMsg.dest_addr = sv->eth_addr;
	reportMsg.dest_port = PORT_ETH;
	// reportMsg.cmd_rc etc depends on what type of report

	// initialize genericMsg for measurement reporting
	/* format:
	 *		cmd_rc = CPU performance
	 *		seq = clock freqency
	 *		arg1 = tempVal[0]
	 *		arg2 = tempVal[2]
	 *		arg3 = currentTempReal
	 *		data[0:3] = reward value
	 *		data[4:7] = average cpu utilization
	 *		data[8:11] = phycpu_0_idle_cntr
	 *		data[12:15] = phycpu_1_idle_cntr
	 *		... etc
	 * */
	genericMsg.flags = 0x07;
	genericMsg.tag = SDP_IPTAG_GENERIC;
	genericMsg.srce_addr = sv->p2p_addr;
	genericMsg.srce_port = (SDP_CONFIG_PORT << 5) + myCoreID;
	genericMsg.dest_addr = sv->eth_addr;
	genericMsg.dest_port = PORT_ETH;
	genericMsg.length = sizeof(sdp_hdr_t) + sizeof(cmd_hdr_t) + 20*sizeof(uint);
}

void initRouter()
{

}

void initTimer()
{
	// setup timer-1;
	spin1_set_timer_tick(TIMER1_TICK_PERIOD_US);
	spin1_callback_on(TIMER_TICK, hTimer1, PRIORITY_TIMER);

	// then setup timer-2
	setupTimer2(TIMER2_TICK_PERIOD_US, hTimer2);
}

/*_____________________________________________________________________*/
