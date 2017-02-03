 #ifndef IRemote_h
#define IRemote_h

#include "Timers.h"

#define SENDING_IR      1
#define RECEIVING_IR    1

#define SEND            0
#define RECEIVE         0

#define rec             6

class decode_results {
	public:
		int                    overflow;
		unsigned int           value;
		volatile unsigned int  *rawbuf;
		int                    rawlen;
};

class SENDIR {
public:
  SENDIR  ()  { }
  void  byteHead (unsigned int time);
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

int  MATCH (int measured,  int desired)	;
int  MATCH_MARKER  (int measured_ticks, int desired_us) ;
int  MATCH_BREAK (int measured_ticks, int desired_us) ;

#endif
