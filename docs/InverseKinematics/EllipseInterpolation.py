import math


class Vector:
    def __init__(self, x: float, y: float, z: float):
        self.x: float = x
        self.y: float = y
        self.z: float = z

    def __str__(self):
        return f"({self.x}, {self.y}, {self.z})"


Point = Vector

a: Point = Point(0, 0, 0)  # start point
b: Point = Point(1,1, 1)  # end point

c: Point = Point((a.x + b.x) / 2.0, (a.y + b.y) / 2.0, (a.z + b.z) / 2.0)  # center point

semiMajorAxis: Vector = Vector(b.x - c.x, b.y - c.y, b.z - c.z)  # major half axis (from c to b)

simplePerpendicular: Vector = Vector(-semiMajorAxis.z, 0, semiMajorAxis.x)  # simply generated per. point

# cross product of two previous ones
minorX: float = semiMajorAxis.y * simplePerpendicular.z - semiMajorAxis.z * simplePerpendicular.y
minorY: float = semiMajorAxis.z * simplePerpendicular.x - semiMajorAxis.x * simplePerpendicular.z
minorZ: float = semiMajorAxis.x * simplePerpendicular.y - semiMajorAxis.y * simplePerpendicular.x

semiMinorAxis: Vector = Vector(minorX, minorY, minorZ)

# axes sizes
majorAxisMagnitude: float = (semiMajorAxis.x ** 2 + semiMajorAxis.y ** 2 + semiMajorAxis.z ** 2) ** 0.5
minorAxisMagnitude: float = (semiMinorAxis.x ** 2 + semiMinorAxis.y ** 2 + semiMinorAxis.z ** 2) ** 0.5

# make minor axis equal to a portion of the major axis
semiMinorAxis.x = (semiMinorAxis.x / minorAxisMagnitude) * (2.0 * majorAxisMagnitude)
semiMinorAxis.y = (semiMinorAxis.y / minorAxisMagnitude) * (2.0 * majorAxisMagnitude)
semiMinorAxis.z = (semiMinorAxis.z / minorAxisMagnitude) * (2.0 * majorAxisMagnitude)

# print(semiMajorAxis)
# print(semiMinorAxis)

stepSize: float = 0.1
param: int = 0

while param <= int(math.pi / stepSize):
    inverseParam: int = int(math.pi / stepSize) - param
    inverseParam *= stepSize

    xP = c.x + semiMajorAxis.x * math.cos(inverseParam) + semiMinorAxis.x * math.sin(inverseParam)
    yP = c.y + semiMajorAxis.y * math.cos(inverseParam) + semiMinorAxis.y * math.sin(inverseParam)
    zP = c.z + semiMajorAxis.z * math.cos(inverseParam) + semiMinorAxis.z * math.sin(inverseParam)

    ellipsePoint: Point = Point(xP, yP, zP)
    print(ellipsePoint)

    param += 1
