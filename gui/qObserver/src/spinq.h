#ifndef SPINQ_H
#define SPINQ_H

#include <QObject>

#define DEF_SEND_PORT	17893		// tidak bisa diganti dengan yang lain

typedef struct sdp_hdr		// SDP header
{
  uchar flags;
  uchar tag;
  uchar dest_port;
  uchar srce_port;
  quint16 dest_addr;
  quint16 srce_addr;
} sdp_hdr_t;

typedef struct cmd_hdr		// Command header
{
  quint16 cmd_rc;
  quint16 seq;
  quint32 arg1;
  quint32 arg2;
  quint32 arg3;
} cmd_hdr_t;


// This is how genericMsg is used for reporting measurement:
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
typedef struct qParam
{
	quint16 CPUperf;
	quint16 freq;
	quint32 tempVal0;
	quint32 tempVal2;
	float tempReal;			// Note: stdfix is not supported in host machine!
	qint32 rewardVal;
	quint32 avgCPU;
	quint32 cpuIdle[18];
} qParam_t;


#define N_FREQ_ITEM				16	// from 10 to 250
#define N_STATES				N_FREQ_ITEM
#define SDP_CMD_SET_FREQ		0xf2e0
#define SDP_CMD_GOTO_STATE		0x6070
#define SDP_CMD_SEND_CPU_MAP	0xaaaa
#define SDP_CMD_REPORT_PLL		0x1234
#define SDP_CMD_RUNQ			0x2345
#define SDP_CMD_STOPQ			0x3456

// SDP-related parameters - Please check its consistency!!!
#define SDP_IPTAG_REPORT		1
#define SDP_UDP_PORT_REPORT		20001
#define SDP_IPTAG_GENERIC		2
#define SDP_UDP_PORT_GENERIC	20002
#define SDP_IPTAG_ERR_DEBUG		3
#define SDP_UDP_PORT_ERR_DEBUG	20003
#define SDP_HOST_IP				0x02F0A8C0	// 192.168.240.2, dibalik!

#define SDP_CONFIG_PORT			7		// port-7 has a special purpose, usually related with ETH
#define SDP_CMD_PORT			6
#define SDP_TIMEOUT				10		// as recommended


const uchar X_CHIPS[48] = {0,1,0,1,2,3,4,2,3,4,5,0,1,2,3,4,5,6,0,1,2,3,4,5,
							6,7,1,2,3,4,5,6,7,2,3,4,5,6,7,3,4,5,6,7,4,5,6,7};
const uchar Y_CHIPS[48] = {0,0,1,1,0,0,0,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,
							3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7};

#endif // SPINQ_H
