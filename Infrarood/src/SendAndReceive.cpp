#include "InfraRed.h"
#include "Timers.h"
#include "stdbool.h"

#define  BIT_AMOUNT 32
#define  BYTE_HEADER 4500
#define  BYTE_HEADER_REST 2250
#define  PLAYER_Y 2000
#define  PLAYER_X 1750
#define  BLOCK_Y 1500
#define  BLOCK_X 1250
#define  BOM_Y 1000
#define  BOM_X 750
#define  BIT_MARKER 500
#define  BIT_ONE 300
#define  BIT_ZERO 150

#if SENDING_IR
void SENDIR::sendingIR(unsigned int data, int bits) {
  enableIR1(38);
  byteHead(BYTE_HEADER);
  byteHeadRest(BYTE_HEADER_REST);
  // byteSortHead(PLAYER_Y);
  // byteHeadRest(BYTE_HEADER_REST);

  unsigned long mask = 1UL;

  for(int i = 0; i < bits; i++) {
    if (data & mask){
      bitmarker(BIT_MARKER);
      one(BIT_ONE);
    } else {
      bitmarker(BIT_MARKER);
      zero(BIT_ZERO);
    }
    mask <<= 1;
  }
  bitmarker(BIT_MARKER);
  zero(0);
}
#endif

#if RECEIVING_IR
bool RECEIVEIR::receivingIR(decode_results *results) {
  long data = 0;
  int offset = 1;
  // Check header "mark"
  if (!MATCH_MARK(results->rawbuf[offset], BYTE_HEADER))  return false ;
  offset++;

  // Check header "space"
  if (!MATCH_SPACE(results->rawbuf[offset], BYTE_HEADER_REST))  return false ;
  offset++;

  // Build the data
  for (int i = 0;  i < 8;  i++) {
    // Check data "mark"
    if (!MATCH_MARK(results->rawbuf[offset], BIT_MARKER))  return false ;
    offset++;
        // Suppend this bit
    if      (MATCH_SPACE(results->rawbuf[offset], BIT_ONE ))  data = (data << 1) | 1 ;
    else if (MATCH_SPACE(results->rawbuf[offset], BIT_ZERO))  data = (data << 1) | 0 ;
    else                                                            return false ;
    offset++;
  }

  // Success
  results->bits        = BIT_AMOUNT;
  results->value       = data;

  return true;
}
#endif
