#pragma once

// stance position coordinates
#define STANCE_X 160.458f
#define STANCE_Y 58.604f
#define STANCE_Z 0.0f

// names for points utilized during different robot movements
enum StepStages
{
  // movement forward
  STRAIGHT_FORWARD,
  STRAIGHT_BACKWARDS,

  // rotation clockwise
  ROTATION_FORWARD,
  ROTATION_BACKWARDS
};
