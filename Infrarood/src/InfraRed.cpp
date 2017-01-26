#include <avr/interrupt.h>

#define IR_GLOBAL
#	include "InfraRed.h"
#	include "Timers.h"
#undef IR_GLOBAL


int  MATCH (int measured,  int desired)
{
  bool passed = ((measured >= TICKS_LOW(desired)) && (measured <= TICKS_HIGH(desired)));
  if (passed)
 	return passed;
}
// checks how long the infrared stream comes in, and compares it to what we want
int  MATCH_MARKER (int measured_ticks,  int desired_us) {
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS)) && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}

// check how long the break takes, and compares it to what we want
int  MATCH_BREAK (int measured_ticks,  int desired_us){
  bool passed = ((measured_ticks >= TICKS_LOW (desired_us - MARK_EXCESS)) && (measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS)));
 	return passed;
}

ISR (TIMER2_COMPA_vect)
{
	TIMER_RESET;

	// Read if IR Receiver -> SPACE [xmt LED off] or a MARK [xmt LED on]
	// digitalRead() is very slow. Optimisation is possible, but makes the code unportable
	uint8_t  irdata = (uint8_t)digitalRead(irparams.IRpin);

	irparams.timer++;  // One more 50uS tick
  if(irparams.rawlen >= RAWBUF) {
    irparams.rcvstate = STATE_OVERFLOW ;  // Buffer overflow
  }

	switch(irparams.rcvstate) {
		//......................................................................
		case STATE_IDLE: // In the middle of a gap
			if (irdata == Byte_Header || irdata == Bit_Marker) {
				if (irparams.timer < GAP_TICKS)  {  // Not big enough to be a gap.
					irparams.timer = 0;

				} else {
					// Gap just ended; Record duration; Start recording transmission
          irparams.overflow                  = false;
					irparams.rawlen                    = 0;
					irparams.rawbuf[irparams.rawlen++] = irparams.timer;
					irparams.timer                     = 0;
					irparams.rcvstate                  = STATE_MARK;
				}
			}
			break;
		//......................................................................
		case STATE_MARK:  // Timing Mark
			if (irdata == Byte_Header_Rest || irdata == Bit_One || irdata == Bit_Zero) {   // Mark ended; Record time
				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
				irparams.timer                     = 0;
				irparams.rcvstate                  = STATE_SPACE;
			}
			break;
		//......................................................................
		case STATE_SPACE:  // Timing Space
			if (irdata == Byte_Header || irdata == Bit_Marker) {  // Space just ended; Record time
				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
				irparams.timer                     = 0;
				irparams.rcvstate                  = STATE_MARK;

			} else if (irparams.timer > GAP_TICKS) {  // Space
					// A long Space, indicates gap between codes
					// Flag the current code as ready for processing
					// Switch to STOP
					// Don't reset timer; keep counting Space width
					irparams.rcvstate = STATE_STOP;
			}
			break;
		//......................................................................
		case STATE_STOP:  // Waiting; Measuring Gap
		 	if (irdata == Byte_Header || irdata == Bit_Marker)  irparams.timer = 0 ;  // Reset gap timer
		 	break;
		//......................................................................
    case STATE_OVERFLOW:  // Flag up a read overflow; Stop the State Machine
    irparams.overflow = true;
    irparams.rcvstate = STATE_STOP;
    break;
	}
}
// ISR (TIMER2_COMPA_vect)
// {
// 	TIMER_RESET;
//
// 	// Read if IR Receiver -> SPACE [xmt LED off] or a MARK [xmt LED on]
// 	// digitalRead() is very slow. Optimisation is possible, but makes the code unportable
// 	uint8_t  irdata = (uint8_t)digitalRead(irparams.IRpin);
//
// 	irparams.timer++;  // One more 50uS tick // Buffer overflow
//
// 	switch(irparams.rcvstate) {
// 		//......................................................................
// 		case STATE_IDLE: // In the middle of a gap
// 			if (irdata == MARK) {
// 				if (irparams.timer < GAP_TICKS)  {  // Not big enough to be a gap.
// 					irparams.timer = 0;
//
// 				} else {
// 					// Gap just ended; Record duration; Start recording transmission
// 					irparams.rawlen                    = 0;
// 					irparams.rawbuf[irparams.rawlen++] = irparams.timer;
// 					irparams.timer                     = 0;
// 					irparams.rcvstate                  = STATE_MARK;
// 				}
// 			}
// 			break;
// 		//......................................................................
// 		case STATE_MARK:  // Timing Mark
// 			if (irdata == SPACE) {   // Mark ended; Record time
// 				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
// 				irparams.timer                     = 0;
// 				irparams.rcvstate                  = STATE_SPACE;
// 			}
// 			break;
// 		//......................................................................
// 		case STATE_SPACE:  // Timing Space
// 			if (irdata == MARK) {  // Space just ended; Record time
// 				irparams.rawbuf[irparams.rawlen++] = irparams.timer;
// 				irparams.timer                     = 0;
// 				irparams.rcvstate                  = STATE_MARK;
//
// 			} else if (irparams.timer > GAP_TICKS) {  // Space
// 					// A long Space, indicates gap between codes
// 					// Flag the current code as ready for processing
// 					// Switch to STOP
// 					// Don't reset timer; keep counting Space width
// 					irparams.rcvstate = STATE_STOP;
// 			}
// 			break;
// 		//......................................................................
// 		case STATE_STOP:  // Waiting; Measuring Gap
// 		 	if (irdata == MARK)  irparams.timer = 0 ;  // Reset gap timer
// 		 	break;
// 		//......................................................................
// 	}}
