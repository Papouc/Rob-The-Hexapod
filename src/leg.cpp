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

  // move to default pos
  moveToPos(IK_DEFUALT_X, IK_DEFAULT_Y);

  startX = goalX = IK_DEFUALT_X;
  startY = goalY = IK_DEFAULT_Y;

  lineParam = (int)(1.0f / interpolationStep);
}

float Leg::power(float num)
{
  return num * num;
}

bool Leg::isInGoal()
{
  return lineParam >= (int)(1.0f / interpolationStep);
}

void Leg::setGoal(float x, float y)
{
  goalX = x;
  goalY = y;

  dirX = goalX - startX;
  dirY = goalY - startY;

  lineParam = 0;
}

void Leg::moveToPos(float x, float y)
{
  // main input processing
  float input = sqrtf(power(x) + power(y));
  float gammaAngle = degrees(atanf(x / y));

  // femur positioning
  float femurAngle = acosf((power(DIAG_LINK) - power(MAIN_LINK) - power(input)) / (-2.0f * MAIN_LINK * input));
  femurAngle = degrees(femurAngle);

  // tibia positioning
  float beta1 = acosf((power(input) - power(DIAG_LINK) - power(MAIN_LINK)) / (-2.0f * DIAG_LINK * MAIN_LINK));
  beta1 = degrees(beta1);

  float beta2 = asinf((SEC_DIAG_LINK * sinf(radians(SEC_DIAG_ANGLE))) / DIAG_LINK);
  beta2 = degrees(beta2);

  float tibiaAngle = beta1 + beta2;

  // gamma correction
  if (y < 0.0f)
  {
    // gamma angle is negative at this point
    gammaAngle = 90.0f + (90.0f + gammaAngle);
  }

  // recalculate real servo motor angles
  float orientation = (onRightSide ? -1.0f : 1.0f);

  float femurToWrite = femurBaseAngle + orientation * (180.0f - (gammaAngle + femurAngle));
  float tibiaToWrite = tibiaBaseAngle + orientation * (90.0f - tibiaAngle);

  femurRamp.go(femurToWrite, rampDuration);
  tibiaRamp.go(tibiaToWrite, rampDuration);
}

void Leg::update()
{
  if (isInGoal())
  {
    // next movement will start where the previous ended
    startX = goalX;
    startY = goalY;
  }
  else if (femurRamp.isFinished() && tibiaRamp.isFinished())
  {
    // move to the next point on the line
    float pointX = startX + (lineParam * interpolationStep) * dirX;
    float pointY = startY + (lineParam * interpolationStep) * dirY;

    moveToPos(pointX, pointY);
    lineParam++;
  }

  // update interpolation
  coxa.write(coxaRamp.update());
  femur.write(femurRamp.update());
  tibia.write(tibiaRamp.update());
}
