#pragma once

#define BT_BAUD_RATE 9600

class BtController
{
private:
  bool connected = false;

public:
  void initialize();
  char read();
};
