#include "IRremote.h"
#include "IRremoteInt.h"

#define  BIT_AMOUNT 32
#define  BYTE_HEADER 8000
#define  BYTE_HEADER_REST 4000
#define  BIT_MARKER 1000
#define  BIT_ONE 2000
#define  BIT_ZERO 500

#if SENDING_IR
void SENDIR::sendingIR(unsigned long data, int bits) {
  enableIR(38);
  byteHead(BYTE_HEADER);
  byteHeadRest(BYTE_HEADER_REST);

  for(unsigned long mask = 1UL << (bits - 1); mask; mask >>= 1) {
    if (data & mask){
      bit(BIT_MARKER);
      one(BIT_ONE);
    } else {
      bit(BIT_MARKER);
      zero(BIT_ZERO);
    }
  }
  bit(BIT_MARKER);
  zero(0);
}
#endif

#if RECEIVING_IR
bool RECEIVEIR::receivingIR(decode_results *results) {
  long data = 0;
  int offset = 1;

}
#endif
