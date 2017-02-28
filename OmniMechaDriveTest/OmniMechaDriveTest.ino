#include <Servo.h>

//PARAMETERS
//
//set which ports the motors will be attached to here
const unsigned char motorPort[4] = {3, 5, 6, 9};

//DO NOT TOUCH
double motorPower[4] = {0.0, 0.0, 0.0, 0.0};
Servo motor[4];

float w = 90 * PI/180.0/1000;
float theta = 0;
long checkpoint, elapsed;

//SETUP
//    --0--
//   /     \
//  3       1
//   \     /
//    --2--

//moves the vehicle according to a vector
void move_omni(double x, double y, double power = 1.0)
{
  double m = sqrt(x*x + y*y);
  double p = max(-1.0, min(1.0, power));
  
  motorPower[0] = p * x/m;
  motorPower[1] = p * -y/m;
  motorPower[2] = p * -x/m;
  motorPower[3] = p * y/m;
}
//rotates the vehicle in place
void rotate_omni(double power)
{
  double p = max(-1.0, min(1.0, power));
  
  motorPower[0] = p;
  motorPower[1] = p;
  motorPower[2] = p;
  motorPower[3] = p;
}

//SETUP
//   |-----|
//   0     1
//   |     |
//   2     3
//   |-----|

//moves the vehicle according to a vector
void move_mechanum(double x, double y, double power = 1.0)
{
  double m = abs(x) + abs(y);
  double p = max(-1.0, min(1.0, power));
  
  motorPower[0] =  (y + x)/m * p;
  motorPower[1] = (-y + x)/m * p;
  motorPower[2] =  (y - x)/m * p;
  motorPower[3] = (-y - x)/m * p;
}
//rotates the vehicle in place
void rotate_mechanum(double power)
{
  double p = max(-1.0, min(1.0, power));
  
  motorPower[0] = p;
  motorPower[1] = p;
  motorPower[2] = p;
  motorPower[3] = p;
}

//call to update state of motors
void drive()
{
  for(int i = 0; i < 4; ++i)
  {
    motor[i].write((motorPower[i] + 1.0) * 90.0);
    //Serial.print((motorPower[i] + 1.0) * 90.0);
    //if(i < 3) Serial.print(" ");
    //else Serial.println("");
  }
}

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 4; ++i) motor[i].attach(motorPort[i]);
  checkpoint = millis();
}

void loop() {
  elapsed = millis() - checkpoint;
  checkpoint += elapsed;
  theta += w * elapsed;
  while(theta >= 2 * PI) theta -= 2 * PI;
  //delay(50);
  move_omni(cos(theta), sin(theta), 1);
  int i = 0;
  do
  {
    Serial.print(motorPower[i] + 3 * (float(i)-1.5));
    Serial.print(" ");
    Serial.print(3*(float(i)-1.5));
    Serial.print(" ");
    ++i;
  } while(i < 4);
//  Serial.print(theta);
//  Serial.print(" ");
//  Serial.print(cos(theta) * 10);
//  Serial.print(" ");
//  Serial.print(sin(theta)* 10);

  Serial.println("");
  drive();
}
