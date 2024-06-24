#pragma once
#include "servoArgs.h"
#include <Ramp.h>
#include <Servo.h>

// leg hardware params (in mm)
#define MAIN_LINK 78.720
#define BOTTOM_LINK 84.052
#define DIAG_LINK 136.917
#define SEC_DIAG_LINK 107.656
#define SEC_DIAG_ANGLE 121.100 // degrees

#define IK_DEFUALT_X 96.252
#define IK_DEFAULT_Y 18.653

class Leg
{
private:
  Servo coxa;
  Servo femur;
  Servo tibia;

  rampFloat coxaRamp;
  rampFloat femurRamp;
  rampFloat tibiaRamp;

  bool onRightSide;

  float power(float num);

public:
  int rampDuration = 120;

  Leg();
  Leg(ServoArgs coxaSrv, ServoArgs femurSrv, ServoArgs tibiaSrv, bool rightSide);

  void update();
  void moveToPos(float x, float y);
};
