#include "InfraRed.h"
#include "Timers.h"

int  RECEIVEIR::checkData(decode_results *results)
{
	results->rawbuf   = irparams.rawbuf;
	results->rawlen   = irparams.rawlen;

	results->overflow = irparams.overflow;

	if (irparams.rcvstate != STATE_STOP)  return false ;

	if (receivingIR(results)) {
    return true;
  }

	goOn();
	return false;
}

//+=============================================================================
RECEIVEIR::RECEIVEIR (int IRpin){
	irparams.IRpin = IRpin;
}

void RECEIVEIR::enableIR0(){
	cli();
	// Setup pulse clock timer interrupt
	// Prescale /8 (16M/8 = 0.5 microseconds per tick)
	// Therefore, the timer interval can range from 0.5 to 128 microseconds
	// Depending on the reset value (255 to 0)
	TIMER_CONFIG_NORMAL();

	// Timer2 Overflow Interrupt Enable
	TIMER_ENABLE_INTR;

	TIMER_RESET;

	sei();  // enable interrupts

	// Initialize state machine variables
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;

	// Set pin modes
	pinMode(irparams.IRpin, INPUT);
}
void RECEIVEIR::goOn ( ){
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;
}

bool  RECEIVEIR::isIdle ( ){
	if(irparams.rcvstate == STATE_IDLE){
		return true;
	} else if(irparams.rcvstate == STATE_STOP){
		return false;
	}
 }
