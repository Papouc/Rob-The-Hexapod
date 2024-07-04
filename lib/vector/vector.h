#pragma once

struct Vector
{
  float x;
  float y;
  float z;

  Vector operator+(const Vector &other);
  Vector operator-(const Vector &other);
  Vector operator*(const float num);
  Vector cross(Vector other);
  float getMagnitude();
};
