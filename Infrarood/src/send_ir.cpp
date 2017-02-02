#include "InfraRed.h"
#include "Timers.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "stdio.h"
#include "avr/io.h"
#include "avr/delay.h"

void SENDIR::enableIR1(int khz){
  TIMER_DISABLE_INTR;
  DDRD |= (1 << TIMER_PWM_PIN);
  PORTD &= ~(1 << TIMER_PWM_PIN);
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
