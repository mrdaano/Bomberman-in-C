#ifndef IRemote_h
#define IRemote_h

#include "IRremoteInt.h"

#define SENDING_IR      1
#define RECEIVING_IR    1

class decode_results
{
	public:
		unsigned int           address;      // Used by Panasonic & Sharp [16-bits]
		unsigned long          value;        // Decoded value [max 32-bits]
		int                    bits;         // Number of bits in decoded value
		volatile unsigned int  *rawbuf;      // Raw intervals in 50uS ticks
		int                    rawlen;       // Number of records in rawbuf
		int                    overflow;     // true iff IR raw code too long
};

class SENDIR {
public:
  SENDIR  ()  { }
  void  byteHead (unsigned int usec);
  void  byteHeadRest (unsigned int usec);
  void  bit (unsigned int usec);
  void  one (unsigned int usec);
  void  zero  (unsigned int usec);
  void  enableIR1 (int khz);
  void  our_delay (unsigned long uSec);
  void  sendingIR (unsigned long data, int bits);
};

class RECEIVEIR {
public:
  RECEIVEIR(){}
  int decode (decode_results *results);
  void enableIR0();
};
#endif
