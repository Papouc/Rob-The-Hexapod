#pragma once
#include "leg.h"
#include "stepStages.h"

#define LEG_CNT 6
#define INITIAL_WAIT 2000

#define STANCE_X 160.458f
#define STANCE_Y 58.604f
#define STANCE_Z 0.0f

class Body
{
private:
  Leg legs[LEG_CNT];
  unsigned long startTime;
  bool odd;
  bool isStanding;

  void walkForward();
  void rotateAround();

  Vector getMotionVector(int legIndex, StepStages stage);

public:
  void initialize();
  void update();
};
