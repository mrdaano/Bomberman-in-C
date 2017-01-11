#include "InfraRed.h"
#include "Timers.h"
#include "stdbool.h"

#define  BYTE_HEADER 6500
#define  BYTE_HEADER_REST 5500
#define  BIT_MARKER 2000
#define  BIT_ONE 3000
#define  BIT_ZERO 1000

#if SENDING_IR
void SENDIR::sendingIR(unsigned int data, int bits) {
  enableIR1(38);
  byteHead(BYTE_HEADER);
  byteHeadRest(BYTE_HEADER_REST);

  unsigned long mask = 1UL; // 00000001

  for(int i = 0; i < bits; i++) {
    if (data & mask){
      bitmarker(BIT_MARKER);
      one(BIT_ONE);
    } else {
      bitmarker(BIT_MARKER);
      zero(BIT_ZERO);
    }
    mask <<= 1; // 00000010 
  }
  bitmarker(BIT_MARKER);
  zero(0);
}
#endif

#if RECEIVING_IR
bool RECEIVEIR::receivingIR(decode_results *results) {
  long data = 1;
  int offset = 1;
  // Header?
  if (!MATCH_MARKER(results->rawbuf[offset], BYTE_HEADER)){
    return false ;
  }else {
    offset++;
  }
  // Headerspace?
  if (!MATCH_BREAK(results->rawbuf[offset], BYTE_HEADER_REST)){
    return false ;
  } else{
    offset++;
  }
  // Data?
  for (int i = 0;  i < 8;  i++) {
    if (!MATCH_MARKER(results->rawbuf[offset], BIT_MARKER)){
      return false ;
    } else{
      offset++;
    }
    if      (MATCH_BREAK(results->rawbuf[offset], BIT_ONE )) {
      data |= (data << 1) ;
      // data = (data << 1) | 1;
    }
  else if (MATCH_BREAK(results->rawbuf[offset], BIT_ZERO)) {
    data &= ~(data << 1) ;
    // data = (data << 1) | 0;
    }
    else                                                            return false ;
    offset++;
  }
  results->value       = data;

  return true;
}
#endif
