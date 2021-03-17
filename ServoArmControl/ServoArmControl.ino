#include <Servo.h>
#include <math.h>
#include <Ramp.h>

#define SERVO_BASE 9
#define SERVO_ELBOW 10 //PWM pin that is connected to the servo

Servo servo_base;
Servo servo_elbow;
ramp Ramp;
const int num_Readings = 100;
const int side;
uint8_t baseReadings[num_Readings];
int baseReadIndex = 0;
int baseTotal = 0;
int baseAverage = 0;
int baseAngle = 90;
uint8_t elbowReadings[num_Readings];
int elbowReadIndex = 0;
int elbowTotal = 0;
int elbowAverage = 0;
int elbowAngle = 90;

int pointListX[] = {20, 25, 35, 40,45,50};
int pointListY[] = {5, 5, 5, 5,5,5};


void armBaseRunning(double radius);
void armElbowRunning(double radius);
int angleGen(int x);
bool manualControl = false;

void setup()
{

  int pointRadius = 0;
  servo_base.attach(SERVO_BASE, 0, 2500);
  servo_elbow.attach(SERVO_ELBOW, 0, 2500);
  pinMode(A1, INPUT);
  pinMode(A4, INPUT);
  Serial.begin(9600);
  Serial.println("started");
  for (int thisReading = 0; thisReading < num_Readings; thisReading++)
  {
    baseReadings[thisReading] = 0;
    elbowReadings[thisReading] = 0;
  }
}

void loop()
{
    
  for (int i = 0; i < 6; i++)
  {

    double pointRadius = sqrt(((pointListX[i] * pointListX[i])) + (pointListY[i] * pointListY[i]));
    if (pointRadius <= 120)
    {
      armBaseRunning(pointRadius);

      armElbowRunning(pointRadius);
       delay(500);
    }
   
  }
}

void armBaseRunning(double radius)
{
  if (manualControl == true)
  {
    baseTotal = baseTotal - baseReadings[baseReadIndex];
    baseReadings[baseReadIndex] = analogRead(A1);
    baseTotal = baseTotal + baseReadings[baseReadIndex];
    baseReadIndex = baseReadIndex + 1;
    if (baseReadIndex >= num_Readings)
    {
      baseReadIndex = 0;
    }

    baseAverage = baseTotal / num_Readings;
    int baseAngle = angleGen(baseAverage);
  }
  double baseAngle = (acos(((radius * radius)) / ( radius * 120)))* (180/3.14);
  
  Serial.println("executing base");
   Serial.println(radius);
  Serial.println(baseAngle);
  //servo_base.write(Ramp.go(baseAngle,1000));
  servo_base.write(baseAngle);
}

void armElbowRunning(double radius)
{ 
  
  if (manualControl == true)
  {
    elbowTotal = elbowTotal - elbowReadings[elbowReadIndex];
    elbowReadings[elbowReadIndex] = analogRead(A4);
    elbowTotal = elbowTotal + elbowReadings[elbowReadIndex];
    elbowReadIndex = elbowReadIndex + 1;
    if (elbowReadIndex >= num_Readings)
    {
      elbowReadIndex = 0;
    }

    elbowAverage = elbowTotal / num_Readings;
    int elbowAngle = angleGen(elbowAverage);
  }
  double elbowAngle = (acos(((7200) - (radius * radius)) / (7200))) * (180/3.14);
    Serial.println("executing arm");
    Serial.println(radius);
  Serial.println(elbowAngle);
  //servo_elbow.write(Ramp.go(elbowAngle,1000));
  servo_elbow.write(elbowAngle);
}

int angleGen(int x)
{

  return ((sqrt(32400 - (x * x))));
}
