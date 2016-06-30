/* Contains all definition and variable declarations that are related
 * with standard SpiNNaker mechanism through SpiNNaker API.
 * */
#ifndef STDSPINAPI_H
#define STDSPINAPI_H

#include <spin1_api.h>

#define TIMER1_TICK_PERIOD_US 	1000000
#define TIMER2_TICK_PERIOD_US	100000

#define PRIORITY_MCPL			-1
#define PRIORITY_SDP			0
#define PRIORITY_DMA			1
#define PRIORITY_TIMER			2
#define PRIORITY_PROCESSING		3
#define PRIORITY_LOWER			4
#define PRIORITY_IDLE			PRIORITY_LOWER

// SDP-related parameters
#define SDP_IPTAG_REPORT		1
#define SDP_UDP_PORT_REPORT		20001
#define SDP_IPTAG_GENERIC		2
#define SDP_UDP_PORT_GENERIC	20002
#define SDP_IPTAG_ERR_DEBUG		3
#define SDP_UDP_PORT_ERR_DEBUG	20003
#define SDP_HOST_IP				0x02F0A8C0	// 192.168.240.2, dibalik!

#define SDP_CONFIG_PORT			7		// port-7 has a special purpose, usually related with ETH

#define SDP_TIMEOUT				10		// as recommended

sdp_msg_t reportMsg;			// for sending Q-table
sdp_msg_t genericMsg;			// for sending measurement
sdp_msg_t debugMsg;				//

// forward declaration
void initSDP();
void initRouter();
void initIPTag();
void initCallback();
void initTimer();

void hDMADone(uint tid, uint tag);
void hTimer1(uint tick, uint Unused);
void hTimer2(uint tick, uint Unused);
void hSDP(uint mBox, uint port);
void hMCPL(uint key, uint payload);

uint myCoreID;

#endif // STDSPINAPI_H

