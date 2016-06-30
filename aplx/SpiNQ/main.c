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


	// greetings
    io_printf(IO_STD, "[SpiNQ-v%d.%d] running @ core-%d id-%d\n",
			  MAJOR_VERSION, MINOR_VERSION, myCoreID, sark_app_id());

	// then start the event
	spin1_start(SYNC_NOWAIT);

	terminateProfiler();
}
