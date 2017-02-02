#include "header.h"

// Get the grid x position of a player or bomb
int getPlayerXGrid(int x) {
  if (x >= 0 && x <= 25) {
    return 1;
  } else if (x >= 25 && x <= 50) {
    return 2;
  } else if (x >= 50 && x <= 75) {
    return 3;
  } else if (x >= 75 && x <= 100) {
    return 4;
  } else if (x >= 100 && x <= 125) {
    return 5;
  } else if (x >= 125 && x <= 150) {
    return 6;
  } else if (x >= 150 && x <= 175) {
    return 7;
  } else if (x >= 175 && x <= 200) {
    return 8;
  } else if (x >= 200 && x <= 225) {
    return 9;
  }

  return 10;
}

// Get the grid y position of a player or bomb
int getPlayerYGrid(int y) {
  if (y >= 0 && y <= 27) {
    return 1;
  } else if (y >= 27 && y <= 54) {
    return 2;
  } else if (y >= 54 && y <= 81) {
    return 3;
  } else if (y >= 81 && y <= 108) {
    return 4;
  } else if (y >= 108 && y <= 135) {
    return 5;
  } else if (y >= 135 && y <= 162) {
    return 6;
  } else if (y >= 162 && y <= 189) {
    return 7;
  }

  return 8;
}
