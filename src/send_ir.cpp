#include "IRemote.h"
#include "IRremoteInt.h"

void SENDIR::enableIR1(int khz){
  TIMER_DISABLE_INTR;
  pinMode(TIMER_PWM_PIN, OUTPUT);
	digitalWrite(TIMER_PWM_PIN, LOW);
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

void SENDIR::bit(unsigned int time) {
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

void SENDIR::our_delay(unsigned long uSec){
  if(uSec > 5) {
    unsigned long start = micros();
    unsigned long endMicros = start + uSec - 5;
    if(endMicros < start) {
      while(micros() > start){}
    }
    while(micros() < endMicros){}
  }
}
