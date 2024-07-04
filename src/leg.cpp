#include "leg.h"
#include "Arduino.h"

Leg::Leg() {}
Leg::Leg(ServoArgs coxaSrv, ServoArgs femurSrv, ServoArgs tibiaSrv, bool rightSide)
    : coxaBaseAngle(coxaSrv.baseAngle),
      femurBaseAngle(femurSrv.baseAngle),
      tibiaBaseAngle(tibiaSrv.baseAngle),
      onRightSide(rightSide)
{
  coxa.attach(coxaSrv.pin, coxaSrv.minUs, coxaSrv.maxUs);
  femur.attach(femurSrv.pin, femurSrv.minUs, femurSrv.maxUs);
  tibia.attach(tibiaSrv.pin, tibiaSrv.minUs, tibiaSrv.maxUs);

  // instantly set servo base positions
  coxaRamp.go(coxaBaseAngle);
  femurRamp.go(femurBaseAngle);
  tibiaRamp.go(tibiaBaseAngle);

  // compute beta2 angle just once (remains constant)
  beta2 = asinf((SEC_DIAG_LINK * sinf(radians(SEC_DIAG_ANGLE))) / DIAG_LINK);
  beta2 = degrees(beta2);

  // set default pos
  start = goal = { IK_DEFUALT_X, IK_DEFAULT_Y, IK_DEFAULT_Z };

  // set param so that leg is already in it's goal position (max + 1)
  param = (int)(paramCap / stepSize) + 1;
}

float Leg::power(float num)
{
  return num * num;
}

bool Leg::isInGoal()
{
  bool inPlace = coxaRamp.isFinished() && femurRamp.isFinished() && tibiaRamp.isFinished();
  return param > (int)(paramCap / stepSize) && inPlace;
}

void Leg::setPosLine(Vector pos)
{
  // keep around 10 points on the line
  interpolationType = LINE;
  paramCap = 1.0f;
  stepSize = 0.1f;

  // apply new goal
  goal = pos;

  // calculate line direction
  direction = goal - start;

  pathLength = direction.getMagnitude();
  param = 0;
}

void Leg::setPosEllipse(Vector pos)
{
  // keep around 10 points on the ellipse
  interpolationType = ELLIPSE;
  paramCap = PI;
  stepSize = 0.314;

  goal = pos;

  // point between start and goal
  centerPoint = (start + goal) * 0.5f;

  // major axis (length is already correct)
  semiMajorAxis = goal - centerPoint;

  // simple to get perpendicular vector (flip components + change sign)
  Vector simplePerpendicular = { -semiMajorAxis.z, 0.0f, semiMajorAxis.x };

  // get real perpendicular vector using cross product
  semiMinorAxis = semiMajorAxis.cross(simplePerpendicular);

  // adjust length of minor axis
  float majorAxisLen = semiMajorAxis.getMagnitude();
  float minorAxisLen = semiMinorAxis.getMagnitude();

  // make it a unit vector + multiply it by half of the legth of major axis
  semiMinorAxis = semiMinorAxis * (1.0f / minorAxisLen);
  semiMinorAxis = semiMinorAxis * (majorAxisLen / 2.0f);

  // update minor axis length
  minorAxisLen = majorAxisLen / 2.0f;

  // get lenght (as 1/2 of ellipse's circumference)
  pathLength = (PI / 4) * (minorAxisLen + majorAxisLen + sqrtf(2 * (power(minorAxisLen) + power(majorAxisLen))));
  param = 0;
}

void Leg::moveToPos(Vector pos)
{
  // 3D IK part
  float thetaAngle = degrees(atanf(pos.z / pos.x));
  float x2D = sqrtf(power(pos.x) + power(pos.z));

  // continue as it was 2D IK part
  // compensate for coxa length
  x2D = x2D - COXA_LINK;
  float y2D = pos.y;

  float bSide = sqrtf(power(x2D) + power(y2D));
  float gammaAngle = degrees(atanf(x2D / y2D));

  // femur positioning
  float femurAngle = acosf((power(DIAG_LINK) - power(MAIN_LINK) - power(bSide)) / (-2.0f * MAIN_LINK * bSide));
  femurAngle = degrees(femurAngle);

  // tibia positioning
  float beta1 = acosf((power(bSide) - power(DIAG_LINK) - power(MAIN_LINK)) / (-2.0f * DIAG_LINK * MAIN_LINK));
  beta1 = degrees(beta1);

  float tibiaAngle = beta1 + beta2;

  // gamma correction
  if (y2D < 0.0f)
  {
    // gamma angle is negative at this point
    gammaAngle = 90.0f + (90.0f + gammaAngle);
  }

  // recalculate real servo motor angles
  float orientation = (onRightSide ? -1.0f : 1.0f);

  float coxaToWrite = coxaBaseAngle + thetaAngle;
  float femurToWrite = femurBaseAngle + orientation * (180.0f - (gammaAngle + femurAngle));
  float tibiaToWrite = tibiaBaseAngle + orientation * (90.0f - tibiaAngle);

  // keep constant movement speed (path / velocity / number of points = time to move between two points)
  int rampTime = (pathLength / rampSpeed) / (int)(paramCap / stepSize);

  coxaRamp.go(coxaToWrite, rampTime);
  femurRamp.go(femurToWrite, rampTime);
  tibiaRamp.go(tibiaToWrite, rampTime);
}

void Leg::update()
{
  if (isInGoal())
  {
    // next movement will start where the previous ended
    start = goal;
  }
  else if (coxaRamp.isFinished() && femurRamp.isFinished() && tibiaRamp.isFinished())
  {
    // move to the next point on the line
    Vector newPos;

    if (interpolationType == LINE)
    {
      float paramValue = param * stepSize;

      // line parametric equations
      newPos.x = start.x + paramValue * direction.x;
      newPos.y = start.y + paramValue * direction.y;
      newPos.z = start.z + paramValue * direction.z;
    }
    else
    {
      // elipse parameterizations direction is opposite to the line parameterizations
      int inverseParam = (int)(paramCap / stepSize) - param;
      float paramValue = inverseParam * stepSize;

      // ellipse parametric equations
      newPos.x = centerPoint.x + semiMajorAxis.x * cosf(paramValue) + semiMinorAxis.x * sinf(paramValue);
      newPos.y = centerPoint.y + semiMajorAxis.y * cosf(paramValue) + semiMinorAxis.y * sinf(paramValue);
      newPos.z = centerPoint.z + semiMajorAxis.z * cosf(paramValue) + semiMinorAxis.z * sinf(paramValue);
    }

    moveToPos(newPos);
    param++;
  }

  // properly round to angles
  int coxaAngle = roundf(coxaRamp.update());
  int femurAngle = roundf(femurRamp.update());
  int tibiaAngle = roundf(tibiaRamp.update());

  // update interpolation
  coxa.write(coxaAngle);
  femur.write(femurAngle);
  tibia.write(tibiaAngle);
}
