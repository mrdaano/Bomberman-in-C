#include "header.h"

bool checkHitWall(PLAYER *p, int side) {
  if (((p->x + 20) == 245) && (side == 1)) {
    return true;
  }
  //Wall coordinates
  int walls[20][2] = {
    {2,2}, {2,4}, {2,6}, {2,8}, {2,10},
    {4,2}, {4,4}, {4,6}, {4,8}, {4,10},
    {6,2}, {6,4}, {6,6}, {6,8}, {6,10},
    {8,2}, {8,4}, {8,6}, {8,8}, {8,10}
  };

  //Get player position
  int xGridB = 0, yGridB = 0;
  int xGridE = 0, yGridE = 0;
  if (side == 1) {
    xGridB = getPlayerXGrid(p->x + 25);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x + 25);
    yGridE = getPlayerYGrid(p->y + 26);
  } else if (side == 2) {
    xGridB = getPlayerXGrid(p->x);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x);
    yGridE = getPlayerYGrid(p->y + 25);
  } else if (side == 3) {
    xGridB = getPlayerXGrid(p->x + 1);
    yGridB = getPlayerYGrid(p->y + 25);
    xGridE = getPlayerXGrid(p->x + 24);
    yGridE = getPlayerYGrid(p->y + 26);
  } else if (side == 4) {
    xGridB = getPlayerXGrid(p->x + 1);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x + 24);
    yGridE = getPlayerYGrid(p->y);
  }

  //Check if player is hitting a wall
  for (int i = 0; i < 20; i++) {
    int x = walls[i][1];
    int y = walls[i][0];
      if (((xGridB == x) && (yGridB == y)) || ((xGridE == x) && (yGridE == y))) {
      return true;
    } else if (((xGridB == x) && (yGridB == y)) && ((xGridE == x) && (yGridE == y))) {
      return true;
    }

  }
  //Check if player is hitting a crate
  for (int i = 0; i < 30; i++) {
    int x = temp[i][1];
    int y = temp[i][0];
      if (((xGridB == x) && (yGridB == y)) || ((xGridE == x) && (yGridE == y))) {
      return true;
    } else if (((xGridB == x) && (yGridB == y)) && ((xGridE == x) && (yGridE == y))) {
      return true;
    }

  }

  return false;
}
