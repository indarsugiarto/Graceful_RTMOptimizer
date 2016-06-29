#include "SpiNQ.h"

void c_main()
{
	myCoreID = sark_core_id();
    initIPTag();
    initSDP();
    initCallback();

    io_printf(IO_STD, "[SpiNQ-v%d.%d] running @ core-%d id-%d\n",
              myCoreID, sark_app_id());

    // let's randomly initialize currentState
    uchar x = sark_rand() % N_STATES;
    uchar y = sark_rand() % N_STATES;

	spin1_start(SYNC_NOWAIT);
}
