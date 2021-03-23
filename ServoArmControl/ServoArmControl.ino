#include <Servo.h>
#include <math.h>
#include <Ramp.h>

#define SERVO_BASE 10
#define SERVO_ELBOW 9

const int baseArmLength = 108;
const int elbowArmLength = 72;

class triangles
{
public:
  double radius_ = 1;
  double a1, a2, b1, b2, c0 = 0;
  double cosAngle = 0;

  triangles(int x, int y)
  {
    radius_ = sqrt(sq(x) + sq(y));
    Serial.println(x);
    Serial.println(y);
    a1 = atan2(y,x) * (180 / 3.14159);
    b1 = 90-a1;
    cosAngle = (sq(baseArmLength) + sq(elbowArmLength) - sq(radius_)) / (baseArmLength * 2 * elbowArmLength);
    c0 = acos(cosAngle) * 180 / 3.14159;
    cosAngle = (sq(radius_) + sq(elbowArmLength) - sq(baseArmLength)) / (radius_ * 2 * elbowArmLength);
    a2 = acos(cosAngle) * 180 / 3.14159;
    b2 = 180 - (c0 + a2);

    if (b1 == -45.00003814697265625)
    {
      b1 = 90;
    }

    Serial.println(radius_);
    Serial.println(a1);
    Serial.println(b1);
    Serial.println(a2);
    Serial.println(b2);
    Serial.println(c0);
  };
};

Servo servo_base;
Servo servo_elbow;

void baseServo(triangles Space);
void elbowServo(triangles Space);

void setup()
{

  Serial.begin(115200);
  Serial.println("started");
  servo_base.attach(SERVO_BASE, 500, 2500);
  servo_elbow.attach(SERVO_ELBOW, 500, 2500);
}

void loop()
{
  for (int i = 0; i < 150; i++)
  { 
    Serial.println(i);
    triangles test(i, 100);
    baseServo(test);
    elbowServo(test);
    delay(5);
  }
}

void baseServo(triangles Space)
{
  double baseServoAngle = Space.b1 + Space.b2;
  servo_base.write((int)baseServoAngle);
  Serial.println("base angle");
  Serial.println((int)baseServoAngle);
}
void elbowServo(triangles Space)
{
  if (Space.c0 > 45.)
  {
    servo_elbow.write((int)Space.c0);
    Serial.println("elbow angle");
    Serial.println((int)Space.c0);
  }
}
