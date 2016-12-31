#ifndef IRemote_h
#define IRemote_h

#include "Timers.h"

#define SENDING_IR      1
#define RECEIVING_IR    1

class decode_results {
	public:
		unsigned long          value;        // Decoded value [max 32-bits]
		int                    bits;         // Number of bits in decoded value
		volatile unsigned int  *rawbuf;      // Raw intervals in 50uS ticks
		int                    rawlen;       // Number of records in rawbuf
		int                    overflow;     // true iff IR raw code too long
};

class SENDIR {
public:
  SENDIR  ()  { }
  void  byteHead (unsigned int time);
	void  byteSortHead(unsigned int time);
  void  byteHeadRest (unsigned int time);
  void  bitmarker (unsigned int time);
  void  one (unsigned int time);
  void  zero  (unsigned int time);
  void  enableIR1 (int khz);
  void  our_delay (unsigned int uSecs);
  void  sendingIR (unsigned int data, int bits);
};

class RECEIVEIR {
public:
  RECEIVEIR(int IRpin);
  int checkData (decode_results *results);
  void enableIR0();
  void goOn();
	bool isIdle();
private:
  bool receivingIR(decode_results *results);
};

#define DEBUG  0

//------------------------------------------------------------------------------
// Debug directives
//
#if DEBUG
#	define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
#	define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#	define DBG_PRINT(...)
#	define DBG_PRINTLN(...)
#endif

int  MATCH       (int measured, int desired) ;
int  MATCH_MARK  (int measured_ticks, int desired_us) ;
int  MATCH_SPACE (int measured_ticks, int desired_us) ;
#endif
