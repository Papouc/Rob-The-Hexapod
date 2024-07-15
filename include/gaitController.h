#pragma once
#include "leg.h"
#include "stepStages.h"

#ifndef LEG_CNT
  #define LEG_CNT 6
#endif

class GaitController
{
private:
  Leg *legs;
  bool odd;

  bool trioTranslated = false;

  void applyMotion(StepStages oddStep[], StepStages evenStep[]);
  Vector getMotionVector(int legIndex, StepStages stage);

public:
  bool inTranslationMode = false;

  GaitController();
  GaitController(Leg legs[]);

  void switchSides();

  // individual gait parts implementations
  void walkForward();
  void walkBackward();
  void rotateCW();
  void rotateCCW();
  void walkDiagLeftFW();
  void walkDiagRightFW();
  void walkDiagLeftBW();
  void walkDiagRightBW();
};
