#include "profiler.h"

/* isr_for_timer2()
*
*  This interrupt service routine is called upon countdown of the processor's
*  primary timer to zero. In response, a callback is scheduled.
*/
INT_HANDLER isr_for_timer2 (void)
{
	// Clear timer interrupt
	//tc[T2_INT_CLR] = 1;				// --> spin1_api way
	tc[T2_INT_CLR] = (uint) tc;			// --> interrupt example way

	// Increment simulation "time" and call the "longer" handler
	ticks2++;

	//Indar: TODO if timer 2 fires............
	spin1_schedule_callback(cbackTimer2, ticks2, 0, PRIORITY_TIMER);

	// Ack VIC: Tell VIC we're done
	//vic[VIC_VADDR] = 1;				// --> spin1_api way
	vic[VIC_VADDR] = (uint) vic;		// --> interrupt example way
}

