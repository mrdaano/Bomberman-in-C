#include "InfraRed.h"
#include "Timers.h"
#include "stdbool.h"

#define  BYTE_HEADER 6000
#define  BYTE_HEADER_REST 4500
#define  BIT_MARKER 1000
#define  BIT_ONE 2000
#define  BIT_ZERO 1000

#if SENDING_IR
void SENDIR::sendingIR(unsigned int data, int bits) {
  enableIR1(38);
  byteHead(BYTE_HEADER);
  byteHeadRest(BYTE_HEADER_REST);

  //unsigned long mask = 1UL; // 00000001

  for(unsigned long  mask = 1UL << (bits - 1);  mask;  mask >>= 1) {
    if (data & mask){
      bitmarker(BIT_MARKER);
      one(BIT_ONE);
    } else {
      bitmarker(BIT_MARKER);
      zero(BIT_ZERO);
    }
    // mask <<= 1; // 00000010
  }
  bitmarker(BIT_MARKER);
  zero(0);
}
#endif

#if RECEIVING_IR
bool RECEIVEIR::receivingIR(decode_results *results) {
  long data = 0;
  int datanr = 0;
  // Header?
  if (!MATCH_MARKER(results->rawbuf[datanr], BYTE_HEADER)){
    return false ;
  }
    datanr++;
  // Headerspace?
  if (!MATCH_BREAK(results->rawbuf[datanr], BYTE_HEADER_REST)){
    return false ;
  }
    datanr++;
  // Data?
  for (int i = 0;  i < 8;  i++) {
    if (!MATCH_MARKER(results->rawbuf[datanr], BIT_MARKER)){
      return false ;
    }
      datanr++;
    if      (MATCH_BREAK(results->rawbuf[datanr], BIT_ONE)) {
      data = (data << 1) | 1;
    }
  else if (MATCH_BREAK(results->rawbuf[datanr], BIT_ZERO)) {
    data = (data << 1) | 0;
    }
    else                                                            return false ;
    datanr++;
  }
  results->value       = data;

  return true;
}
#endif
