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
  StepStages oddStep[] = { STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW };
  StepStages evenStep[] = { STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW };

  applyMotion(oddStep, evenStep);
}

void GaitController::walkBackward()
{
  StepStages oddStep[] = { STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW };
  StepStages evenStep[] = { STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW, STRAIGHT_FW, STRAIGHT_BW };

  applyMotion(oddStep, evenStep);
}

void GaitController::rotateCW()
{
  StepStages oddStep[] = { ROTATION_FW, ROTATION_BW, ROTATION_FW, ROTATION_FW, ROTATION_BW, ROTATION_FW };
  StepStages evenStep[] = { ROTATION_BW, ROTATION_FW, ROTATION_BW, ROTATION_BW, ROTATION_FW, ROTATION_BW };

  applyMotion(oddStep, evenStep);
}

void GaitController::rotateCCW()
{
  StepStages oddStep[] = { ROTATION_BW, ROTATION_FW, ROTATION_BW, ROTATION_BW, ROTATION_FW, ROTATION_BW };
  StepStages evenStep[] = { ROTATION_FW, ROTATION_BW, ROTATION_FW, ROTATION_FW, ROTATION_BW, ROTATION_FW };

  applyMotion(oddStep, evenStep);
}

void GaitController::walkDiagLeftFW()
{
  StepStages oddStep[] = { DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW };
  StepStages evenStep[] = { DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW };

  applyMotion(oddStep, evenStep);
}

void GaitController::walkDiagRightFW()
{
  StepStages oddStep[] = { DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_AWAY_BW };
  StepStages evenStep[] = { DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_AWAY_FW };

  applyMotion(oddStep, evenStep);
}

void GaitController::walkDiagLeftBW()
{
  StepStages oddStep[] = { DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_AWAY_FW };
  StepStages evenStep[] = { DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_AWAY_BW };

  applyMotion(oddStep, evenStep);
}

void GaitController::walkDiagRightBW()
{
  StepStages oddStep[] = { DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW };
  StepStages evenStep[] = { DIAG_AWAY_FW, DIAG_AWAY_BW, DIAG_AWAY_FW, DIAG_INSIDE_BW, DIAG_INSIDE_FW, DIAG_INSIDE_BW };

  applyMotion(oddStep, evenStep);
}

void GaitController::applyMotion(StepStages oddStep[], StepStages evenStep[])
{
  StepStages *usedStep = odd ? oddStep : evenStep;

  // assign destination point to each leg
  for (int i = 0; i < LEG_CNT; i++)
  {
    if (odd && (i == 0 || i == 2 || i == 4))
    {
      // trio of legs raised during odd step
      legs[i].setPosEllipse(getMotionVector(i, usedStep[i]));
    }
    else if (!odd && (i == 1 || i == 3 || i == 5))
    {
      // trio of legs raised during even step
      legs[i].setPosEllipse(getMotionVector(i, usedStep[i]));
    }
    else
    {
      legs[i].setPosLine(getMotionVector(i, usedStep[i]));
    }
  }
}

Vector GaitController::getMotionVector(int legIndex, StepStages stage)
{
  Vector front, mid, rear;

  // points in each legs 3D space (calculated by hand in advance)
  // would be smart to add some form on non-linear equation solver to eliminate the need
  // to recaluclate this after every minor stance change
  switch (stage)
  {
    case STRAIGHT_FW:
      front = { 185.207f, STANCE_Y, 24.749f };
      mid = { 160.458f, STANCE_Y, 35.0f };
      rear = { 135.709f, STANCE_Y, 24.749f };
      break;
    case STRAIGHT_BW:
      front = { 135.709f, STANCE_Y, -24.749f };
      mid = { 160.458f, STANCE_Y, -35.0f };
      rear = { 185.207, STANCE_Y, -24.749f };
      break;
    case ROTATION_FW:
      front = { 155.384f, STANCE_Y, 34.630f };
      mid = { 188.582f, STANCE_Y, 34.924f };
      rear = { 160.947f, STANCE_Y, 34.997f };
      break;
    case ROTATION_BW:
      front = { 160.947f, STANCE_Y, -34.997f };
      mid = { 188.582f, STANCE_Y, -34.924f };
      rear = { 155.384f, STANCE_Y, -34.630f };
      break;
    case DIAG_AWAY_FW:
      front = { 195.458f, STANCE_Y, 0.0f };
      mid = { 185.207f, STANCE_Y, 24.749f };
      rear = { 160.458f, STANCE_Y, 35.0f };
      break;
    case DIAG_AWAY_BW:
      front = { 125.458f, STANCE_Y, 0.0f };
      mid = { 135.709f, STANCE_Y, -24.749f };
      rear = { 160.458f, STANCE_Y, -35.0f };
      break;
    case DIAG_INSIDE_FW:
      front = { 160.458f, STANCE_Y, 35.0f };
      mid = { 135.709f, STANCE_Y, 24.749f };
      rear = { 125.458f, STANCE_Y, 0.0f };
      break;
    case DIAG_INSIDE_BW:
      front = { 160.458f, STANCE_Y, -35.0f };
      mid = { 185.207f, STANCE_Y, -24.749f };
      rear = { 195.458f, STANCE_Y, 0.0f };
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
