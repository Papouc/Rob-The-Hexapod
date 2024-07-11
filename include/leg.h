#pragma once
#include "interpolationType.h"
#include "servoArgs.h"
#include "vector.h"
#include <Ramp.h>
#include <Servo.h>

// leg hardware params (in mm)
#define MAIN_LINK 78.720f
#define BOTTOM_LINK 84.052f
#define DIAG_LINK 136.917f
#define COXA_LINK 79.0f
#define SEC_DIAG_LINK 107.656f
#define SEC_DIAG_ANGLE 121.100f // degrees

#define IK_DEFUALT_X 175.252f
#define IK_DEFAULT_Y 18.653f
#define IK_DEFAULT_Z 0.0f

class Leg
{
private:
  // leg setup
  Servo coxa;
  Servo femur;
  Servo tibia;

  rampFloat coxaRamp;
  rampFloat femurRamp;
  rampFloat tibiaRamp;

  // leg params
  int coxaBaseAngle;
  int femurBaseAngle;
  int tibiaBaseAngle;
  bool onRightSide;

  // constant angle part of femur joint
  float beta2;

  // base coordinates
  Vector start;
  Vector goal;

  // interpolation stuff
  InterpolationType interpolationType = LINE;
  float pathLength;
  int movementTime = 60;

  // line
  Vector direction;

  // ellipse
  Vector semiMajorAxis;
  Vector semiMinorAxis;
  Vector centerPoint;

  // parameter settings
  float stepSize = 0.1f;
  float paramCap = 1.0f;
  int param;

  float power(float num);
  void moveToPos(Vector pos);

public:
#ifdef CONST_VELOCITY
  // only valid for constant speed movement
  float rampSpeed = 0.15f;
#endif

  Leg();
  Leg(ServoArgs coxaSrv, ServoArgs femurSrv, ServoArgs tibiaSrv, bool rightSide);

  void update();
  bool isInGoal();
  void setPosLine(Vector pos);
  void setPosEllipse(Vector pos);
};
