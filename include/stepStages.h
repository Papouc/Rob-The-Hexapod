#pragma once

// stance position coordinates
#define STANCE_X 160.458f
#define STANCE_Y 58.604f
#define STANCE_Z 0.0f

// names for points utilized during different robot movements
enum StepStages
{
  // movement forward
  STRAIGHT_FW,
  STRAIGHT_BW,

  // rotation clockwise
  ROTATION_FW,
  ROTATION_BW,

  // diagonal movement
  DIAG_AWAY_FW,
  DIAG_AWAY_BW,
  DIAG_INSIDE_FW,
  DIAG_INSIDE_BW
};
