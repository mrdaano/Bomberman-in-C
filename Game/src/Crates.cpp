#include "header.h"

int temp [30][2]; // Array to store the choosen crates for a single game
int possiblePositions[66][2] = {
  {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9}, {1,10},
  {2,3}, {2,5}, {2,7}, {2,9},
  {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, {3,8}, {3,9}, {3,10},
  {4,1}, {4,3}, {4,5}, {4,7}, {4,9},
  {5,1}, {5,2}, {5,3}, {5,4}, {5,5}, {5,6}, {5,7}, {5,8}, {5,9}, {5,10},
  {6,1}, {6,3}, {6,5}, {6,7}, {6,9},
  {7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}, {7,7}, {7,8}, {7,9}, {7,10},
  {8,1}, {8,3}, {8,5}, {8,7}, {8,9},
  {9,1}, {9,2}, {9,3}, {9,4}, {9,5}, {9,6}, {9,7}, {9,8}, {9,9}
}; // All possible positions for a crate
int prev[30];

void addCrate(int crate, int r1){
  // Add a crate to the temp array
    temp[crate][0] = possiblePositions[r1][0];
    temp[crate][1] = possiblePositions[r1][1];

  // Draw crate on screen
  lcd.fillRect(temp[crate][1] * 25 - 25, temp[crate][0] * 27 - 27, 25, 27, RGB(139,69,19));
  lcd.fillRect(temp[crate][1] * 25 - 22, temp[crate][0] * 27 - 24, 19, 21, RGB(160,82,45));
  lcd.fillTriangle(temp[crate][1] * 25 - 19, temp[crate][0] * 27 - 23,
                   temp[crate][1] * 25 - 6, temp[crate][0] * 27 - 8,
                   temp[crate][1] * 25 - 6, temp[crate][0] * 27 - 22, RGB(139,69,19));
  lcd.fillTriangle(temp[crate][1] * 25 - 20, temp[crate][0] * 27 - 20,
                   temp[crate][1] * 25 - 20, temp[crate][0] * 27 - 6,
                   temp[crate][1] * 25 - 7, temp[crate][0] * 27 - 5, RGB(139,69,19));
}

void spawnCrates(int crate){
  // Spawn crates on random locations
  randomSeed(analogRead(0));
  // Generate random numbers
  for(int i = 0; i < crate; i++) {
    int r = random(66);
    for(int arr = 0; arr < 30; arr++) {
      if (r == prev[arr]) {
        r = random(66);
        arr = 0;
      }
    }

    prev[i] = r;
    addCrate(i, r);
  }
}
