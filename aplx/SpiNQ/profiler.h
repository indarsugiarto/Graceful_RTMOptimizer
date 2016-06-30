#ifndef PROFILER_H
#define PROFILER_H

/*
 * Note:
 * - percobaan tentang timer bisa dilihat di project tryMyTimer
 * - dari https://www.arm.com/products/processors/classic/arm9/arm968.php
 *   sepertinya maximum frequency-nya adalah 270MHz
 *
 * CPUs use PLL1
 */
#include <spin1_api.h>
#include "stdspinapi.h"

/*---------------------------- regarding Timer 2 --------------------------------*/
// -----------------------
// VIC priorities
// -----------------------
#define TIMER2_SLOT		   8		// Indar: similar to TIMER2_PRIORITY, which must be > 7, according to current spin1_api.h
//void update_VIC (void);
void setupTimer2(uint periodT2, callback_t cback);
void reset_timer2(uint _time);		// Indar: add this
void terminate_timer2 (void);
INT_HANDLER isr_for_timer2 ();

static uint timer2_tick;			// Indar: add this, will be used internally
static uint ticks2;					// This will apparent to user, not timer2_tick
static uint timer_tick;  	        // timer tick period
uint iLoad;							// helper variable, to load the counter
callback_t cbackTimer2;				// callback for timer-2
/*---------------------------- regarding Timer 2 --------------------------------*/
/*-------------------------------------------------------------------------------*/


//#define REPORT_TIMER_TICK_PERIOD_US	1000000	// to get 1s resolution in FREQ_REF_200MHZ
#define REPORT_TIMER_TICK_PERIOD_US	100000	// to get 0.1s resolution in FREQ_REF_200MHZ
#define FREQ_REF_200MHZ				200

// related with frequency scalling
#define lnMemTable					93
#define wdMemTable					3
// memTable format: freq, MS, NS --> with default dv = 2, so that we don't have
// to modify r24
static uchar memTable[lnMemTable][wdMemTable] = {
{10,1,2},
{11,5,11},
{12,5,12},
{13,5,13},
{14,5,14},
{15,1,3},
{16,5,16},
{17,5,17},
{18,5,18},
{19,5,19},
{20,1,4},
{21,5,21},
{22,5,22},
{23,5,23},
{24,5,24},
{25,1,5},
{26,5,26},
{27,5,27},
{28,5,28},
{29,5,29},
{30,1,6},
{31,5,31},
{32,5,32},
{33,5,33},
{34,5,34},
{35,1,7},
{36,5,36},
{37,5,37},
{38,5,38},
{39,5,39},
{40,1,8},
{41,5,41},
{42,5,42},
{43,5,43},
{44,5,44},
{45,1,9},
{46,5,46},
{47,5,47},
{48,5,48},
{49,5,49},
{50,1,10},
{51,5,51},
{52,5,52},
{53,5,53},
{54,5,54},
{55,1,11},
{56,5,56},
{57,5,57},
{58,5,58},
{59,5,59},
{60,1,12},
{61,5,61},
{62,5,62},
{63,5,63},
{65,1,13},
{70,1,14},
{75,1,15},
{80,1,16},
{85,1,17},
{90,1,18},
{95,1,19},
{100,1,20},
{105,1,21},
{110,1,22},
{115,1,23},
{120,1,24},
{125,1,25},
{130,1,26},
{135,1,27},
{140,1,28},
{145,1,29},
{150,1,30},
{155,1,31},
{160,1,32},
{165,1,33},
{170,1,34},
{175,1,35},
{180,1,36},
{185,1,37},
{190,1,38},
{195,1,39},
{200,1,40},
{205,1,41},
{210,1,42},
{215,1,43},
{220,1,44},
{225,1,45},
{230,1,46},
{235,1,47},
{240,1,48},
{245,1,49},
{250,1,50},
{255,1,51},
};

/* global variables */
uint myCoreID;
uchar myPhyCore;
sdp_msg_t reportMsg;						// will be sent via SDP_IPTAG_REPORT
sdp_msg_t debugMsg;							// will be sent via SDP_IPTAG_GENERIC

// reading temperature sensors
uint tempVal[3];							// there are 3 sensors in each chip
uint cpuIdleCntr[18];						// for all cpus
uint myOwnIdleCntr;							// since my flag in r25 is alway on, it gives me ALWAYS zero counts

// PLL and frequency related:
uint _r20, _r21, _r24;						// the original value of r20, r21, and r24
uint r20, r21, r24, r25;					// the current value of r20, r21 and r24 during *this* experiment
uint _freq;									// ref/original frequency
volatile uint _idleCntr;
/*-------------------------------------------------------------------------------------------*/

// function prototypes
void readTemp();

void getFreqParams(uint f, uint *ms, uint *ns);
void changeFreq(uint f, uint replyCode);					// we use uchar to limit the frequency to 255
void changePLL(uint flag);
uint readSpinFreqVal();

void idle(uint arg0, uint arg1);
void disableCPU(uint virtCoreID, uint none);
void enableCPU(uint virtCoreID, uint none);

// initProfiler will set timer-2 and PLL
void initProfiler();
void terminateProfiler();

#endif // PROFILER_H

