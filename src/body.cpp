#include "body.h"
#include "commands.h"
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

  // pass pointer to legs arr to gait controller (arr decays into pointer)
  gaitController = GaitController(legs);

  // reach "before connection" position
  sitDown();
}

void Body::update(char command)
{
  bool allPlaced = true;

  // update positions of all legs
  for (int i = 0; i < LEG_CNT; i++)
  {
    if (!legs[i].isInGoal())
    {
      allPlaced = false;
    }

    legs[i].update();
  }

  if (!allPlaced || command == NO_CMD || command == STOP_CMD)
  {
    // wait until current movement is finsihed (or just do "nothing" :D)
    return;
  }

  // execute active leg trio pre-switch if moving to exactly oppostie direction
  checkDirection(command);

  bool isGait = true;
  switch (command)
  {
    case INITIAL_CONNECT_CMD:
      standUp();
      isGait = false;
      break;
    case DISCONNECT_CMD:
      sitDown();
      isGait = false;
      break;
    case FORWARD_CMD:
      gaitController.walkForward();
      break;
    case BACKWARD_CMD:
      gaitController.walkBackward();
      break;
    case ROT_LEFT_CMD:
      gaitController.rotateCCW();
      break;
    case ROT_RIGHT_CMD:
      gaitController.rotateCW();
      break;
    case FORWARD_LEFT_CMD:
      gaitController.walkDiagLeftFW();
      break;
    case FORWARD_RIGHT_CMD:
      gaitController.walkDiagRightFW();
      break;
    case BACKWARD_LEFT_CMD:
      gaitController.walkDiagLeftBW();
      break;
    case BACKWARD_RIGHT_CMD:
      gaitController.walkDiagRightBW();
      break;
  }

  if (isGait)
  {
    // switch active leg trio after succesfully chosing new movemnt
    gaitController.switchSides();
  }

  lastCommand = command;
}

void Body::checkDirection(char newCommand)
{
  if (newCommand == lastCommand)
  {
    // no need for any adjustments
    return;
  }

  gaitController.switchSides();
}

void Body::standUp()
{
  // initial stand up
  Vector stance = { STANCE_X, STANCE_Y, STANCE_Z };
  for (int i = 0; i < LEG_CNT; i++)
  {
    legs[i].setPosLine(stance);
  }

  isStanding = true;
}

void Body::sitDown()
{
  // put all legs into default position
  Vector defaultPos = { IK_DEFUALT_X, IK_DEFAULT_Y, IK_DEFAULT_Z };
  for (int i = 0; i < LEG_CNT; i++)
  {
    legs[i].setPosLine(defaultPos);
  }

  isStanding = false;
}
