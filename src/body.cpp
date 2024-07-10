#include "body.h"
#include "pinLayout.h"
#include "servoArgs.h"

void Body::initialize()
{
  // left front
  ServoArgs leftFrontCoxa = { .pin = LEFT_FRONT_COXA, .minUs = 564, .maxUs = 2516, .baseAngle = 94 };
  ServoArgs leftFrontFemur = { .pin = LEFT_FRONT_FEMUR, .minUs = 540, .maxUs = 2460, .baseAngle = 98 };
  ServoArgs leftFrontTibia = { .pin = LEFT_FRONT_TIBIA, .minUs = 480, .maxUs = 2416, .baseAngle = 96 };
  legs[0] = Leg(leftFrontCoxa, leftFrontFemur, leftFrontTibia, false);

  // left middle
  ServoArgs leftMidCoxa = { .pin = LEFT_MID_COXA, .minUs = 504, .maxUs = 2504, .baseAngle = 86 };
  ServoArgs leftMidFemur = { .pin = LEFT_MID_FEMUR, .minUs = 540, .maxUs = 2460, .baseAngle = 94 };
  ServoArgs leftMidTibia = { .pin = LEFT_MID_TIBIA, .minUs = 616, .maxUs = 2552, .baseAngle = 88 };
  legs[1] = Leg(leftMidCoxa, leftMidFemur, leftMidTibia, false);

  // left rear
  ServoArgs leftRearCoxa = { .pin = LEFT_REAR_COXA, .minUs = 516, .maxUs = 2532, .baseAngle = 92 };
  ServoArgs leftRearFemur = { .pin = LEFT_REAR_FEMUR, .minUs = 484, .maxUs = 2468, .baseAngle = 92 };
  ServoArgs leftRearTibia = { .pin = LEFT_REAR_TIBIA, .minUs = 592, .maxUs = 2560, .baseAngle = 90 };
  legs[2] = Leg(leftRearCoxa, leftRearFemur, leftRearTibia, false);

  // right front
  ServoArgs rightFrontCoxa = { .pin = RIGHT_FRONT_COXA, .minUs = 500, .maxUs = 2484, .baseAngle = 92 };
  ServoArgs rightFrontFemur = { .pin = RIGHT_FRONT_FEMUR, .minUs = 552, .maxUs = 2552, .baseAngle = 80 };
  ServoArgs rightFrontTibia = { .pin = RIGHT_FRONT_TIBIA, .minUs = 468, .maxUs = 2484, .baseAngle = 88 };
  legs[3] = Leg(rightFrontCoxa, rightFrontFemur, rightFrontTibia, true);

  // right middle
  ServoArgs rightMidCoxa = { .pin = RIGHT_MID_COXA, .minUs = 580, .maxUs = 2532, .baseAngle = 96 };
  ServoArgs rightMidFemur = { .pin = RIGHT_MID_FEMUR, .minUs = 476, .maxUs = 2460, .baseAngle = 90 };
  ServoArgs rightMidTibia = { .pin = RIGHT_MID_TIBIA, .minUs = 488, .maxUs = 2488, .baseAngle = 96 };
  legs[4] = Leg(rightMidCoxa, rightMidFemur, rightMidTibia, true);

  // right rear
  ServoArgs rightRearCoxa = { .pin = RIGHT_REAR_COXA, .minUs = 532, .maxUs = 2516, .baseAngle = 98 };
  ServoArgs rightRearFemur = { .pin = RIGHT_REAR_FEMUR, .minUs = 588, .maxUs = 2540, .baseAngle = 92 };
  ServoArgs rightRearTibia = { .pin = RIGHT_REAR_TIBIA, .minUs = 548, .maxUs = 2500, .baseAngle = 86 };
  legs[5] = Leg(rightRearCoxa, rightRearFemur, rightRearTibia, true);

  // put all legs into default position
  Vector defaultPos = { IK_DEFUALT_X, IK_DEFAULT_Y, IK_DEFAULT_Z };
  for (int i = 0; i < LEG_CNT; i++)
  {
    legs[i].setPosLine(defaultPos);
  }

  startTime = millis();
  isStanding = false;
  odd = true;
}

void Body::update()
{
  bool allPlaced = true;

  for (int i = 0; i < LEG_CNT; i++)
  {
    if (!legs[i].isInGoal())
    {
      allPlaced = false;
    }

    legs[i].update();
  }

  if (millis() - startTime < INITIAL_WAIT || !allPlaced)
  {
    // retain some delay for stabilization during standup
    return;
  }

  if (!isStanding)
  {
    // initial stand up
    Vector stance = { STANCE_X, STANCE_Y, STANCE_Z };
    for (int i = 0; i < LEG_CNT; i++)
    {
      legs[i].setPosLine(stance);
    }

    isStanding = true;
    startTime = millis();
    return;
  }

  if (odd)
  {
    // forward step
    legs[0].setPosEllipse(getMotionVector(0, LEAP_FORWARD));
    legs[2].setPosEllipse(getMotionVector(2, LEAP_FORWARD));
    legs[4].setPosEllipse(getMotionVector(4, LEAP_FORWARD));

    // slide
    legs[1].setPosLine(getMotionVector(1, SLIDE_BACK));
    legs[3].setPosLine(getMotionVector(3, SLIDE_BACK));
    legs[5].setPosLine(getMotionVector(5, SLIDE_BACK));

    startTime = millis();
  }
  else
  {
    // forward step
    legs[1].setPosEllipse(getMotionVector(1, LEAP_FORWARD));
    legs[3].setPosEllipse(getMotionVector(3, LEAP_FORWARD));
    legs[5].setPosEllipse(getMotionVector(5, LEAP_FORWARD));

    // slide
    legs[0].setPosLine(getMotionVector(0, SLIDE_BACK));
    legs[2].setPosLine(getMotionVector(2, SLIDE_BACK));
    legs[4].setPosLine(getMotionVector(4, SLIDE_BACK));

    startTime = millis();
  }

  odd = !odd;
}

Vector Body::getMotionVector(int legIndex, StepStages stage)
{
  Vector front, mid, rear;

  switch (stage)
  {
    case LEAP_FORWARD:
      front = { 202.884f, STANCE_Y, 42.426f };
      mid = { 160.458f, STANCE_Y, 60.0f };
      rear = { 118.032f, STANCE_Y, 42.426f };
      break;
    case SLIDE_BACK:
      front = { 118.032f, STANCE_Y, -42.426f };
      mid = { 160.458f, STANCE_Y, -60.0f };
      rear = { 202.884f, STANCE_Y, -42.426f };
      break;
  }

  Vector result;
  if (legIndex >= 0 && legIndex <= 2)
  {
    // left side (only decide between front, mid and rear)
    if (legIndex == 0)
    {
      result = front;
    }
    else if (legIndex == 1)
    {
      result = mid;
    }
    else if (legIndex == 2)
    {
      result = rear;
    }
  }
  else
  {
    // right side (decide between front, mid and rear + change z orientation)
    if (legIndex == 3)
    {
      result = front;
    }
    else if (legIndex == 4)
    {
      result = mid;
    }
    else if (legIndex == 5)
    {
      result = rear;
    }
    result.z = -result.z;
  }

  return result;
}
