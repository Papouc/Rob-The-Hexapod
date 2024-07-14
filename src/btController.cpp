#include "btController.h"
#include "commands.h"
#include <Arduino.h>

void BtController::initialize()
{
  Serial1.begin(BT_BAUD_RATE);
}

char BtController::read()
{
  // set default return value to "no command"
  char result = NO_CMD;

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
  }

  return result;
}
