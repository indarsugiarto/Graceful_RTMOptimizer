#include "SpiNQ.h"

void c_main()
{
	myCoreID = sark_core_id();
    initIPTag();
    initSDP();

    spin1_set_timer_tick(TIMER_TICK_PERIOD_US);
    spin1_callback_on(TIMER_TICK, hTimer, PRIORITY_TIMER);
    spin1_callback_on(DMA_TRANSFER_DONE, hDMADone, PRIORITY_DMA);
    spin1_callback_on(SDP_PACKET_RX, hSDP, PRIORITY_SDP);
    spin1_callback_on(MCPL_PACKET_RECEIVED, hMCPL, PRIORITY_MCPL);

    io_printf(IO_STD, "[SpiNQ-v%d.%d] running @ core-%d id-%d\n",
              myCoreID, sark_app_id());

	spin1_start(SYNC_NOWAIT);
}
