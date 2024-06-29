#include "body.h"
#include "pinLayout.h"
#include "servoArgs.h"

void Body::initialize()
{
  // left front
  ServoArgs leftFrontCoxa = { .pin = LEFT_FRONT_COXA, .minUs = 564, .maxUs = 2516, .baseAngle = 94 };
  ServoArgs leftFrontFemur = { .pin = LEFT_FRONT_FEMUR, .minUs = 540, .maxUs = 2460, .baseAngle = 4 };
  ServoArgs leftFrontTibia = { .pin = LEFT_FRONT_TIBIA, .minUs = 480, .maxUs = 2416, .baseAngle = 96 };
  mLegs[0] = Leg(leftFrontCoxa, leftFrontFemur, leftFrontTibia, false);

  // left middle
  ServoArgs leftMidCoxa = { .pin = LEFT_MID_COXA, .minUs = 504, .maxUs = 2504, .baseAngle = 86 };
  ServoArgs leftMidFemur = { .pin = LEFT_MID_FEMUR, .minUs = 540, .maxUs = 2460, .baseAngle = 4 };
  ServoArgs leftMidTibia = { .pin = LEFT_MID_TIBIA, .minUs = 616, .maxUs = 2552, .baseAngle = 88 };
  mLegs[1] = Leg(leftMidCoxa, leftMidFemur, leftMidTibia, false);

  // left rear
  ServoArgs leftRearCoxa = { .pin = LEFT_REAR_COXA, .minUs = 516, .maxUs = 2532, .baseAngle = 92 };
  ServoArgs leftRearFemur = { .pin = LEFT_REAR_FEMUR, .minUs = 484, .maxUs = 2468, .baseAngle = 6 };
  ServoArgs leftRearTibia = { .pin = LEFT_REAR_TIBIA, .minUs = 592, .maxUs = 2560, .baseAngle = 90 };
  mLegs[2] = Leg(leftRearCoxa, leftRearFemur, leftRearTibia, false);

  // right front
  ServoArgs rightFrontCoxa = { .pin = RIGHT_FRONT_COXA, .minUs = 500, .maxUs = 2484, .baseAngle = 92 };
  ServoArgs rightFrontFemur = { .pin = RIGHT_FRONT_FEMUR, .minUs = 552, .maxUs = 2552, .baseAngle = 170 };
  ServoArgs rightFrontTibia = { .pin = RIGHT_FRONT_TIBIA, .minUs = 468, .maxUs = 2484, .baseAngle = 86  };
  mLegs[3] = Leg(rightFrontCoxa, rightFrontFemur, rightFrontTibia, true);

  // right middle
  ServoArgs rightMidCoxa = { .pin = RIGHT_MID_COXA, .minUs = 580, .maxUs = 2532, .baseAngle = 96 };
  ServoArgs rightMidFemur = { .pin = RIGHT_MID_FEMUR, .minUs = 476, .maxUs = 2460, .baseAngle = 178 };
  ServoArgs rightMidTibia = { .pin = RIGHT_MID_TIBIA, .minUs = 488, .maxUs = 2488, .baseAngle = 96 };
  mLegs[4] = Leg(rightMidCoxa, rightMidFemur, rightMidTibia, true);

  // right rear
  ServoArgs rightRearCoxa = { .pin = RIGHT_REAR_COXA, .minUs = 532, .maxUs = 2516, .baseAngle = 98 };
  ServoArgs rightRearFemur = { .pin = RIGHT_REAR_FEMUR, .minUs = 588, .maxUs = 2540, .baseAngle = 178 };
  ServoArgs rightRearTibia = { .pin = RIGHT_REAR_TIBIA, .minUs = 548, .maxUs = 2500, .baseAngle = 86 };
  mLegs[5] = Leg(rightRearCoxa, rightRearFemur, rightRearTibia, true);
}

void Body::update()
{
  for (int i = 0; i < LEG_CNT; i++)
  {
    mLegs[i].update();
  }
}
