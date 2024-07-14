#include "body.h"
#include "btController.h"
#include <Arduino.h>

Body heaxapodBody;
BtController btController;

void setup()
{
  Serial.begin(9600);

  heaxapodBody.initialize();
  btController.initialize();
}

void loop()
{
  char command = btController.read();
  heaxapodBody.update(command);
}
