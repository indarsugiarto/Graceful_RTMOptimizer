#include "SpiNQ.h"

void c_main()
{
	myCoreID = sark_core_id();

	// initialization
	sark_srand ((sark_chip_id () << 8) + myCoreID * sv->time_ms); // Init randgen
    initIPTag();
    initSDP();
    initCallback();
	initTimer();
	initProfiler();
	initQ();

	// greetings
    io_printf(IO_STD, "[SpiNQ-v%d.%d] running @ core-%d id-%d\n",
			  MAJOR_VERSION, MINOR_VERSION, myCoreID, sark_app_id());

	/*
	for(uchar virtCPU=0; virtCPU<18; virtCPU++) {
		io_printf(IO_STD, "state CPU-%d = %d\n",virtCPU,sv->vcpu_base[virtCPU].cpu_state);
	}
	*/

	// then start the event
	spin1_start(SYNC_NOWAIT);

	terminateProfiler();
}
