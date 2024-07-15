#include "btController.h"
#include <Arduino.h>

void BtController::initialize()
{
  Serial1.begin(BT_BAUD_RATE);
}

char BtController::read()
{
  // set default return value to "no command"
  char result = lastValidValue;

  // check for data availability
  if (Serial1.available() > 0)
  {
    result = Serial1.read();

    // alter connection status
    if (!connected)
    {
      // first recieved command during this session
      connected = true;
      result = INITIAL_CONNECT_CMD;
    }
    else if (connected && result == DISCONNECT_CMD)
    {
      // disconnect command recieved
      connected = false;
    }
    else if (result == potentialValue)
    {
      currentValueReps++;

      if (currentValueReps >= VALUE_CHANGE_THRESHOLD)
      {
        // threshold exceeded, new value will be returned from now on
        lastValidValue = potentialValue;
      }

      result = lastValidValue;
    }
    else
    {
      // change in value, start counting occurrences of this new value
      potentialValue = result;
      currentValueReps = 0;

      result = lastValidValue;
    }
  }

  return result;
}
