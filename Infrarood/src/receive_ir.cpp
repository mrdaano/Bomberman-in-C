#include "InfraRed.h"
#include "Timers.h"

int  RECEIVEIR::checkData(decode_results *results)
{
	results->rawbuf   = irparams.rawbuf;
	results->rawlen   = irparams.rawlen;

	if (receivingIR(results)) {
		goOn();
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
	TIMER_CONFIG_NORMAL();
	// Timer2 Overflow Interrupt Enable
	TIMER_ENABLE_INTR;
	TIMER_RESET;
	sei();  // enable interrupts

	// Initialize state machine variables
	irparams.rawlen = 0;

	// Set pin modes
	DDRB &= ~(1 << irparams.IRpin);
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
