#pragma once
#include "leg.h"

#define LEG_CNT 6

class Body
{
private:
  Leg mLegs[LEG_CNT];

public:
  void initialize();
  void update();
};
