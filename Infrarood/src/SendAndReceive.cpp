#include "InfraRed.h"
#include "Timers.h"
#include "stdbool.h"

#define  BIT_AMOUNT 32
#define  BYTE_HEADER 6000
#define  BYTE_HEADER_REST 4500
// #define  PLAYER_Y 2000
// #define  PLAYER_X 1750
// #define  BLOCK_Y 1500
// #define  BLOCK_X 1250
// #define  BOM_Y 1000
// #define  BOM_X 750
#define  BIT_MARKER 3000
#define  BIT_ONE 2000
#define  BIT_ZERO 1000

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
  long data = 1;
  int offset = 1;
  // Check header "mark"
  if (!MATCH_MARK(results->rawbuf[offset], BYTE_HEADER)){
    return false ;
  }else {
    offset++;
    Serial.println("hoi");
  }
  // Check header "space"
  if (!MATCH_SPACE(results->rawbuf[offset], BYTE_HEADER_REST)){
    return false ;
  } else{
    offset++;
    Serial.println("hoi1");
  }
  // Build the data
  for (int i = 0;  i < 8;  i++) {
    // Check data "mark"
    if (!MATCH_MARK(results->rawbuf[offset], BIT_MARKER)){
      return false ;
    } else{
      offset++;
      Serial.println("bit");
    }
        // Suppend this bit
    if      (MATCH_SPACE(results->rawbuf[offset], BIT_ONE )) {
      data |= (data << 1) ;
      // data = (data << 1) | 1;
      digitalWrite(8, 1);
      Serial.println("one");
    }
  else if (MATCH_SPACE(results->rawbuf[offset], BIT_ZERO)) {
    data &= ~(data << 1) ;
    // data = (data << 1) | 0;
      digitalWrite(8, 0);
      Serial.println("zero");
    }
    else                                                            return false ;
    offset++;
  }

  // Success
  results->bits        = BIT_AMOUNT;
  results->value       = data;

  return true;
}
#endif
