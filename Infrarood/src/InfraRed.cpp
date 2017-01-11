#include <avr/interrupt.h>

// Defining IR_GLOBAL here allows us to declare the instantiation of global variables
#define IR_GLOBAL
#	include "InfraRed.h"
#	include "Timers.h"
#undef IR_GLOBAL

int  MATCH_MARKER (int measured_ticks,  int desired_us)
{
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS))
                && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}

//+========================================================
// Due to sensor lag, when received, Spaces tend to be 100us too short
//
int  MATCH_BREAK (int measured_ticks,  int desired_us)
{
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS))
                && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}
