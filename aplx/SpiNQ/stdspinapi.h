/* Contains all definition and variable declarations that are related
 * with standard SpiNNaker mechanism through SpiNNaker API.
 * */
#ifndef STDSPINAPI_H
#define STDSPINAPI_H

#include <spin1_api.h>

#define TIMER_TICK_PERIOD_US 	1000000
#define PRIORITY_TIMER			3
#define PRIORITY_PROCESSING		2
#define PRIORITY_SDP			1
#define PRIORITY_MCPL			-1
#define PRIORITY_DMA			0

#define SDP_TAG_REPLY			1
#define SDP_UDP_REPLY_PORT		20000
#define SDP_HOST_IP				0x02F0A8C0	// 192.168.240.2, dibalik!
#define SDP_TAG_RESULT			2
#define SDP_UDP_RESULT_PORT		20001
#define SDP_TAG_DEBUG           3
#define SDP_UDP_DEBUG_PORT      20002

sdp_msg_t reportMsg;			// in python, this will be handled in blocking mode (via native socket)
sdp_msg_t resultMsg;			// in python, this will be handled by QtNetwork.QUdpSocket
sdp_msg_t debugMsg;				// in python, this will be handled by QtNetwork.QUdpSocket

// forward declaration
void initSDP();
void initRouter();
void initIPTag();

void hDMADone(uint tid, uint tag);
void hTimer(uint tick, uint Unused);
void hSDP(uint mBox, uint port);
void hMCPL(uint key, uint payload);

uint myCoreID;

#endif // STDSPINAPI_H

