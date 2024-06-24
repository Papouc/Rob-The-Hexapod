#include "leg.h"
#include "Arduino.h"

Leg::Leg() {}
Leg::Leg(ServoArgs coxaSrv, ServoArgs femurSrv, ServoArgs tibiaSrv, bool rightSide) : onRightSide(rightSide)
{
  coxa.attach(coxaSrv.pin, coxaSrv.minUs, coxaSrv.maxUs);
  femur.attach(femurSrv.pin, femurSrv.minUs, femurSrv.maxUs);
  tibia.attach(tibiaSrv.pin, tibiaSrv.minUs, tibiaSrv.maxUs);
}

float Leg::power(float num)
{
  return num * num;
}

void Leg::moveToPos(float x, float y)
{
  // main input processing
  float input = sqrtf(power(x) + power(y));
  float gammaAngle = degrees(atanf(x / y));

  // femur positioning
  float femurAngle = acosf((power(DIAG_LINK) - power(MAIN_LINK) - power(input)) / (-2.0 * MAIN_LINK * input));
  femurAngle = degrees(femurAngle);

  // tibia positioning
  float beta1 = acosf((power(input) - power(DIAG_LINK) - power(MAIN_LINK)) / (-2.0 * DIAG_LINK * MAIN_LINK));
  beta1 = degrees(beta1);

  float beta2 = asinf((SEC_DIAG_LINK * sinf(radians(SEC_DIAG_ANGLE))) / DIAG_LINK);
  beta2 = degrees(beta2);

  float tibiaAngle = beta1 + beta2;

  // gamma correction
  if (y < 0.0)
  {
    // gamma angle is negative at this point
    gammaAngle = 90.0 + (90.0 + gammaAngle);
  }

  // recalculate real servo motor angles
  float femurToWrite = 90.0 - (180.0 - (gammaAngle + femurAngle));
  float tibiaToWrite = 90.0 - (90.0 - tibiaAngle);

  femurRamp.go(femurToWrite, rampDuration);
  tibiaRamp.go(tibiaToWrite, rampDuration);
}

void Leg::update()
{
  float currentFemur = femurRamp.update();
  float currentTibia = tibiaRamp.update();

  coxa.write(90);
  femur.write(onRightSide ? 180 : 0);
  tibia.write(90);
}
