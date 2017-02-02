#include "InfraRed.h"
#include "Timers.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "stdio.h"
#include "avr/io.h"
#include "avr/delay.h"

// Functies van het zenden van infrarood.
void SENDIR::enableIR1(int khz){
  TIMER_DISABLE_INTR;
  DDRD |= (1 << TIMER_PWM_PIN);
  PORTD &= (1 << TIMER_PWM_PIN);
  TIMER_CONFIG_KHZ(khz);
}

void SENDIR::byteHead(unsigned int time) {
  TIMER_ENABLE_PWM;
  if(time>0) {
    our_delay(time);
  }
}

void SENDIR::byteHeadRest(unsigned int time){
  TIMER_DISABLE_PWM;
  if(time>0){
    our_delay(time);
  }
}

void SENDIR::bitmarker(unsigned int time) {
  TIMER_ENABLE_PWM;
  if(time>0){
    our_delay(time);
  }
}

void SENDIR::one(unsigned int time) {
    TIMER_DISABLE_PWM;
    if(time>0){
      our_delay(time);
    }
}

void SENDIR::zero(unsigned int time) {
  TIMER_DISABLE_PWM;
  if(time>0){
    our_delay(time);
  }
}

void SENDIR::our_delay(unsigned int uSecs){
  _delay_us(uSecs);
}

// Functies van het ontvangen van infrarood.
int  RECEIVEIR::checkData(decode_results *results)
{
	results->rawbuf   = irparams.rawbuf;
	results->rawlen   = irparams.rawlen;

	results->overflow = irparams.overflow;

	if (irparams.rcvstate != STATE_STOP)  return false ;

	if (receivingIR(results)) {
    goOn();
    return true;
  }
	goOn();
	return false;
}

// Selecteren van de infrarood pin
RECEIVEIR::RECEIVEIR (int IRpin){
	irparams.IRpin = IRpin;
}

// Aanzetten van infrarood sensor
void RECEIVEIR::enableIR0(){
	cli(); // disable interrupts
	TIMER_CONFIG_NORMAL();
	TIMER_ENABLE_INTR;
	TIMER_RESET;
	sei();  // enable interrupts

	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;

	// Set pin modes
	DDRD &= ~(1 << 4);
}

// legen voor de volgende infrarood stroom
void RECEIVEIR::goOn ( ){
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;
}
