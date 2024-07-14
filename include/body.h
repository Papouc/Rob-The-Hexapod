#pragma once
#include "leg.h"
#include "stepStages.h"
#include "gaitController.h"

#define LEG_CNT 6
#define INITIAL_WAIT 2000

class Body
{
private:
  Leg legs[LEG_CNT];
  GaitController gaitController;
  bool isStanding;
  char lastCommand;

  void standUp();
  void sitDown();
  
  void checkDirection(char newCommand);

public:
  void initialize();
  void update(char command);
};
