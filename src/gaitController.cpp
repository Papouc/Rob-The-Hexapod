#include "gaitController.h"

GaitController::GaitController() {}
GaitController::GaitController(Leg legs[])
    : legs(legs), odd(true)
{
}

void GaitController::switchSides()
{
  // change currently "active" leg trio
  odd = !odd;
}

void GaitController::walkForward()
{
  if (odd)
  {
    // forward step
    legs[0].setPosEllipse(getMotionVector(0, STRAIGHT_FORWARD));
    legs[2].setPosEllipse(getMotionVector(2, STRAIGHT_FORWARD));
    legs[4].setPosEllipse(getMotionVector(4, STRAIGHT_FORWARD));

    // slide
    legs[1].setPosLine(getMotionVector(1, STRAIGHT_BACKWARDS));
    legs[3].setPosLine(getMotionVector(3, STRAIGHT_BACKWARDS));
    legs[5].setPosLine(getMotionVector(5, STRAIGHT_BACKWARDS));
  }
  else
  {
    // forward step
    legs[1].setPosEllipse(getMotionVector(1, STRAIGHT_FORWARD));
    legs[3].setPosEllipse(getMotionVector(3, STRAIGHT_FORWARD));
    legs[5].setPosEllipse(getMotionVector(5, STRAIGHT_FORWARD));

    // slide
    legs[0].setPosLine(getMotionVector(0, STRAIGHT_BACKWARDS));
    legs[2].setPosLine(getMotionVector(2, STRAIGHT_BACKWARDS));
    legs[4].setPosLine(getMotionVector(4, STRAIGHT_BACKWARDS));
  }
}

void GaitController::walkBackward()
{
  if (odd)
  {
    // forward step
    legs[0].setPosEllipse(getMotionVector(0, STRAIGHT_BACKWARDS));
    legs[2].setPosEllipse(getMotionVector(2, STRAIGHT_BACKWARDS));
    legs[4].setPosEllipse(getMotionVector(4, STRAIGHT_BACKWARDS));

    // slide
    legs[1].setPosLine(getMotionVector(1, STRAIGHT_FORWARD));
    legs[3].setPosLine(getMotionVector(3, STRAIGHT_FORWARD));
    legs[5].setPosLine(getMotionVector(5, STRAIGHT_FORWARD));
  }
  else
  {
    // forward step
    legs[1].setPosEllipse(getMotionVector(1, STRAIGHT_BACKWARDS));
    legs[3].setPosEllipse(getMotionVector(3, STRAIGHT_BACKWARDS));
    legs[5].setPosEllipse(getMotionVector(5, STRAIGHT_BACKWARDS));

    // slide
    legs[0].setPosLine(getMotionVector(0, STRAIGHT_FORWARD));
    legs[2].setPosLine(getMotionVector(2, STRAIGHT_FORWARD));
    legs[4].setPosLine(getMotionVector(4, STRAIGHT_FORWARD));
  }
}

void GaitController::rotateCW()
{
  if (odd)
  {
    // forward step
    legs[0].setPosEllipse(getMotionVector(0, ROTATION_FORWARD));
    legs[2].setPosEllipse(getMotionVector(2, ROTATION_FORWARD));
    legs[4].setPosEllipse(getMotionVector(4, ROTATION_BACKWARDS));

    // slide
    legs[1].setPosLine(getMotionVector(1, ROTATION_BACKWARDS));
    legs[3].setPosLine(getMotionVector(3, ROTATION_FORWARD));
    legs[5].setPosLine(getMotionVector(5, ROTATION_FORWARD));
  }
  else
  {
    // forward step
    legs[1].setPosEllipse(getMotionVector(1, ROTATION_FORWARD));
    legs[3].setPosEllipse(getMotionVector(3, ROTATION_BACKWARDS));
    legs[5].setPosEllipse(getMotionVector(5, ROTATION_BACKWARDS));

    // slide
    legs[0].setPosLine(getMotionVector(0, ROTATION_BACKWARDS));
    legs[2].setPosLine(getMotionVector(2, ROTATION_BACKWARDS));
    legs[4].setPosLine(getMotionVector(4, ROTATION_FORWARD));
  }
}

void GaitController::rotateCCW()
{
  if (odd)
  {
    // forward step
    legs[0].setPosEllipse(getMotionVector(0, ROTATION_BACKWARDS));
    legs[2].setPosEllipse(getMotionVector(2, ROTATION_BACKWARDS));
    legs[4].setPosEllipse(getMotionVector(4, ROTATION_FORWARD));

    // slide
    legs[1].setPosLine(getMotionVector(1, ROTATION_FORWARD));
    legs[3].setPosLine(getMotionVector(3, ROTATION_BACKWARDS));
    legs[5].setPosLine(getMotionVector(5, ROTATION_BACKWARDS));
  }
  else
  {
    // forward step
    legs[1].setPosEllipse(getMotionVector(1, ROTATION_BACKWARDS));
    legs[3].setPosEllipse(getMotionVector(3, ROTATION_FORWARD));
    legs[5].setPosEllipse(getMotionVector(5, ROTATION_FORWARD));

    // slide
    legs[0].setPosLine(getMotionVector(0, ROTATION_FORWARD));
    legs[2].setPosLine(getMotionVector(2, ROTATION_FORWARD));
    legs[4].setPosLine(getMotionVector(4, ROTATION_BACKWARDS));
  }
}

Vector GaitController::getMotionVector(int legIndex, StepStages stage)
{
  Vector front, mid, rear;

  // points in each legs 3D space (calculated by hand in advance)
  switch (stage)
  {
    case STRAIGHT_FORWARD:
      front = { 185.207f, STANCE_Y, 24.749f };
      mid = { 160.458f, STANCE_Y, 35.0f };
      rear = { 135.709f, STANCE_Y, 24.749f };
      break;
    case STRAIGHT_BACKWARDS:
      front = { 135.709f, STANCE_Y, -24.749f };
      mid = { 160.458f, STANCE_Y, -35.0f };
      rear = { 185.207, STANCE_Y, -24.749f };
      break;
    case ROTATION_FORWARD:
      front = { 155.384f, STANCE_Y, 34.630f };
      mid = { 188.582f, STANCE_Y, 34.924f };
      rear = { 160.947f, STANCE_Y, 34.997f };
      break;
    case ROTATION_BACKWARDS:
      front = { 160.947f, STANCE_Y, -34.997f };
      mid = { 188.582f, STANCE_Y, -34.924f };
      rear = { 155.384f, STANCE_Y, -34.630f };
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
