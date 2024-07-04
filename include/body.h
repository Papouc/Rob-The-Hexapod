#pragma once
#include "leg.h"
#include "stepStages.h"

#define LEG_CNT 6
#define INITIAL_WAIT 4000

#define STANCE_X 180.0f
#define STANCE_Y 67.0f
#define STANCE_Z 0.0f

class Body
{
private:
  Leg legs[LEG_CNT];
  unsigned long startTime;
  bool odd;
  bool isStanding;
  StepStages currentStage;

public:
  void initialize();
  void update();
};
