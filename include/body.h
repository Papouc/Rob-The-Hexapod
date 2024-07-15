#pragma once
#include "gaitController.h"
#include "leg.h"
#include "stepStages.h"

#define LEG_CNT 6
#define INITIAL_WAIT 2000

class Body
{
private:
  Leg legs[LEG_CNT];
  GaitController gaitController;
  Vector stance = { STANCE_X, STANCE_Y, STANCE_Z };
  Vector defaultPos = { IK_DEFUALT_X, IK_DEFAULT_Y, IK_DEFAULT_Z };
  bool isStanding;
  char lastCommand;

  int stageCounter;
  void startTranslation(char toCMD);
  void translateToNext();

  void standUp();
  void sitDown();

  bool checkDirection(char newCommand);
  void matchCMDtoMove(char command);

public:
  void initialize();
  void update(char command);
};
