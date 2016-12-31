#ifndef Timers_h
#define Timers_h

#	include <arduino.h>
#include "stdint.h"

#ifdef IR_GLOBAL
#	define EXTERN
#else
#	define EXTERN extern
#endif

#define RAWBUF  101  // Maximum length of raw duration buffer

typedef
	struct {
		// The fields are ordered to reduce memory over caused by struct-padding
		uint8_t       rcvstate;        // State Machine state
		uint8_t       IRpin;         // Pin connected to IR data from detector
		uint8_t       rawlen;          // counter of entries in rawbuf
		unsigned int  timer;           // State timer, counts 50uS ticks.
		unsigned int  rawbuf[RAWBUF];  // raw data
		uint8_t       overflow;        // Raw buffer overflow occurred
	}irparams_t;

// ISR State-Machine : Receiver States
#define STATE_IDLE      2
#define STATE_MARK      3
#define STATE_SPACE     4
#define STATE_STOP      5
#define STATE_OVERFLOW  6

// Allow all parts of the code access to the ISR data
// NB. The data can be changed by the ISR at any time, even mid-function
// Therefore we declare it as "volatile" to stop the compiler/CPU caching it
EXTERN  volatile irparams_t  irparams;

#	define SYSCLOCK  16000000  // main Arduino clock


//------------------------------------------------------------------------------
// Defines for setting and clearing register bits
//
#ifndef cbi
#	define cbi(sfr, bit)  (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#	define sbi(sfr, bit)  (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//------------------------------------------------------------------------------
// Pulse parms are ((X*50)-100) for the Mark and ((X*50)+100) for the Space.
// First MARK is the one after the long gap
// Pulse parameters in uSec
//

// Due to sensor lag, when received, Marks  tend to be 100us too long and
//                                   Spaces tend to be 100us too short
#define MARK_EXCESS    100

// microseconds per clock interrupt tick
#define USECPERTICK    50

// Upper and Lower percentage tolerances in measurements
#define TOLERANCE       25
#define LTOL            (1.0 - (TOLERANCE/100.))
#define UTOL            (1.0 + (TOLERANCE/100.))

// Minimum gap between IR transmissions
#define _GAP            4000
#define GAP_TICKS       (_GAP/USECPERTICK)

#define TICKS_LOW(us)   ((int)(((us)*LTOL/USECPERTICK)))
#define TICKS_HIGH(us)  ((int)(((us)*UTOL/USECPERTICK + 1)))

//------------------------------------------------------------------------------
// IR detector output is active low
#define MARK   0
#define SPACE  1

#define TIMER_PWM_PIN 3

// timer
#define TIMER_RESET
#define TIMER_ENABLE_PWM    (TCCR2A |= (1 << COM2B1))
#define TIMER_DISABLE_PWM   (TCCR2A &= ~(1 << COM2B1))
#define TIMER_ENABLE_INTR   (TIMSK2 |= (1 << OCIE2A))
#define TIMER_DISABLE_INTR  (TIMSK2 = 0)

#define TIMER_CONFIG_KHZ(val) ({ \
	const uint8_t pwmval = SYSCLOCK / 2000 / (val); \
	TCCR2A               |= (1 << WGM20); \
	TCCR2B               |= (1 << WGM22) | (1 << CS20); \
	OCR2A                = pwmval; \
	OCR2B                = pwmval / 3; \
})

#define TIMER_COUNT_TOP  (SYSCLOCK * USECPERTICK / 1000000)

//-----------------
#	define TIMER_CONFIG_NORMAL() ({ \
		TCCR2A |= (1 << WGM21); \
		TCCR2B |= (1 << CS21); \
		OCR2A  = TIMER_COUNT_TOP / 8; \
		TCNT2  = 0; \
	})
#endif
