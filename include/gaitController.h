#pragma once
#include "leg.h"
#include "stepStages.h"

class GaitController
{
private:
  Leg *legs;
  bool odd;

  Vector getMotionVector(int legIndex, StepStages stage);

public:
  
  GaitController();
  GaitController(Leg legs[]);

  void switchSides();
  void walkForward();
  void walkBackward();
  void rotateCW();
  void rotateCCW();
};
