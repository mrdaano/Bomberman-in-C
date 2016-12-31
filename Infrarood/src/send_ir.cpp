#include "InfraRed.h"
#include "Timers.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "stdio.h"
#include "avr/io.h"
#include "avr/delay.h"

void SENDIR::enableIR1(int khz){
  TIMER_DISABLE_INTR;
  pinMode(TIMER_PWM_PIN, OUTPUT);
	digitalWrite(TIMER_PWM_PIN, LOW);
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

void SENDIR::byteSortHead(unsigned int time) {
  TIMER_ENABLE_PWM;
  if(time>0) {
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
