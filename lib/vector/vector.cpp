#include "vector.h"
#include "math.h"

Vector Vector::operator+(const Vector &other)
{
  // add other vector to this vector (this + other)
  Vector result = { x + other.x, y + other.y, z + other.z };
  return result;
}

Vector Vector::operator-(const Vector &other)
{
  // subtract other vector from this vector (this - other)
  Vector result = { x - other.x, y - other.y, z - other.z };
  return result;
}

Vector Vector::operator*(const float num)
{
  // multiply this vector by number (this * num)
  Vector result = { x * num, y * num, z * num };
  return result;
}

Vector Vector::cross(Vector other)
{
  // get cross product between this and other vector (this X other)
  Vector result;
  result.x = y * other.z - z * other.y;
  result.y = z * other.x - x * other.z;
  result.z = x * other.y - y * other.x;
  return result;
}

float Vector::getMagnitude()
{
  return sqrtf((x * x) + (y * y) + (z * z));
}