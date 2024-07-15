#pragma once
#include "commands.h"

#define BT_BAUD_RATE 9600
#define VALUE_CHANGE_THRESHOLD 6

class BtController
{
private:
  char lastValidValue = NO_CMD;
  char potentialValue = NO_CMD;
  unsigned int currentValueReps = 0;

  bool connected = false;

public:
  void initialize();
  char read();
};
