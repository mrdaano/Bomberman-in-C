#include <avr/interrupt.h>

#define IR_GLOBAL
#	include "InfraRed.h"
#	include "Timers.h"
#undef IR_GLOBAL

// checks how long the infrared stream comes in, and compares it to what we want
int  MATCH_MARKER (int measured_ticks,  int desired_us)
{
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS))
                && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}

// check how long the break takes, and compares it to what we want
int  MATCH_BREAK (int measured_ticks,  int desired_us)
{
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS))
                && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}
