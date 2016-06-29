#include "SpiNQ.h"

void initCallback()
{
    spin1_set_timer_tick(TIMER_TICK_PERIOD_US);
    spin1_callback_on(TIMER_TICK, hTimer, PRIORITY_TIMER);
    spin1_callback_on(DMA_TRANSFER_DONE, hDMADone, PRIORITY_DMA);
    spin1_callback_on(SDP_PACKET_RX, hSDP, PRIORITY_SDP);
    spin1_callback_on(MCPL_PACKET_RECEIVED, hMCPL, PRIORITY_MCPL);

    sark_srand ((sark_chip_id () << 8) + myCoreID * sv->time_ms); // Init randgen
}

void initIPTag()
{

}

void initSDP()
{

}

void initRouter()
{

}

// initProfiler: use Timer-2 and change PLL
void initProfiler()
{

}
