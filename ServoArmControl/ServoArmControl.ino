#include <Servo.h>
#include <math.h>
#include <Ramp.h>

#define SERVO_BASE 9
#define SERVO_ELBOW 10 //PWM pin that is connected to the servo

Servo servo_base;
Servo servo_elbow;
ramp Ramp;
const int num_Readings = 100;
uint8_t baseReadings[num_Readings];
int baseReadIndex = 0;
int baseTotal = 0;
int baseAverage = 0;
int baseAngle = 90;
const int baseArmLength = 108;
uint8_t elbowReadings[num_Readings];
int elbowReadIndex = 0;
int elbowTotal = 0;
int elbowAverage = 0;
int elbowAngle = 90;
const int elbowArmLength = 72;

//int pointListX[] = {100, 100, 60, 40};
//int pointListY[] = {100, 50, 60, 20};
int pointListX[] = {50, 100, 50, 10};
int pointListY[] = {150, 100, 50, 100};

void armBaseRunning(double radius, int x, int y);
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
    
  for (int i = 0; i < 4; i++)
  {

    double pointRadius = sqrt(sq(pointListX[i]) + sq(pointListY[i]));
    Serial.print("The Calculated R value: ");
     Serial.print(pointRadius);
  
      armBaseRunning(pointRadius, pointListX[i], pointListY[i] );
     
      armElbowRunning(pointRadius);
       delay(1000);
    
   
  }
}

void armBaseRunning(double radius, int x ,int y)
{
  Serial.println("received r value =");
  Serial.println(radius);
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
  double elbowAngle = (acos((sq(baseArmLength)+sq(elbowArmLength)-sq(radius)) / ( baseArmLength * 2 * elbowArmLength)))* (180/3.14);
  

  double Oac = (acos((sq(elbowArmLength)+sq(radius)-sq(baseArmLength)) / ( elbowArmLength * 2 * radius)))* (180/3.14);
  Serial.println("executing base");

  double Obb = atan(x/y) * 180/3.14;
  Serial.println(Obb);
  double Oab = 180 - elbowAngle - Oac;
  Serial.println(Oab);
  double baseAngle = Oab + Obb;
  
   Serial.println("baseAngle =");
  Serial.print(baseAngle);
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
  double elbowAngle = (acos((sq(baseArmLength)+sq(elbowArmLength)-sq(radius)) / ( baseArmLength * 2 * elbowArmLength)))* (180/3.14);
    Serial.println("elbowAngle =");
  Serial.print(elbowAngle);
  //servo_elbow.write(Ramp.go(elbowAngle,1000));
  servo_elbow.write(elbowAngle);
}

int angleGen(int x)
{

  return ((sqrt(32400 - (x * x))));
}
