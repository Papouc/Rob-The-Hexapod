#include "body.h"
#include <Arduino.h>

Body heaxapodBody;

void setup()
{
  Serial.begin(9600);
  heaxapodBody.initialize();
}

void loop()
{
  heaxapodBody.update();
}
