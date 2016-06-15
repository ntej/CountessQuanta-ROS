#include <Wire.h>
#include <Servo.h>
#include <ros.h>
#include <std_msgs/Empty.h>



#define  FINGER_SERVO_POS           22      //'F'
#define  WRISTS2S_SERVO_POS         24      //'C'
#define  WRISTUPDOWN_SERVO_POS      26      //'A'
#define  WRISTROTATION_SERVO_POS    28      //'W'
#define INIT_FINGERS 123
#define INIT_WRIST_UD 75
#define INIT_WRIST_SIDE2SIDE 60
#define INIT_WRIST_ROTATE 70
Servo fingers;
Servo wristRotation;
Servo wristSide2Side;
Servo wristUpDown;

#define DEVICE_ADDRESS 0x05
#define M1_MODE_REGISTER 0x44
#define M2_MODE_REGISTER 0x47
#define M1_POWER_REGISTER 0x45
#define M2_POWER_REGISTER 0x46

int initialPotVal;
int potVal;
bool iteration = true;

ros::NodeHandle nh;

void messageCb(const std_msgs::Empty& arm_seq_msg)
{
   if(iteration == true)  //arm sequence
  {
   moveToFinalPos();
   delay(2000);
   moveToInitialPos();
  }
}

ros::Subscriber<std_msgs::Empty> sub("run_arm_seq",&messageCb);

void setup() {
  
 Wire.begin();
  Serial.begin(9600);
/*******************************servo******************************************/
  // initialize servos and attaces to pins 
  fingers.attach(FINGER_SERVO_POS);
  wristRotation.attach(WRISTROTATION_SERVO_POS);
  wristSide2Side.attach(WRISTS2S_SERVO_POS);
  wristUpDown.attach(WRISTUPDOWN_SERVO_POS);
  delay(200); // waits to be assign 
  
  //Initialization
  wristUpDown.write(INIT_WRIST_UD); 
  delay(1000);
  fingers.write(INIT_FINGERS);       // Finger Motor range: 90 (close) to 180 (open)
  delay(1000);
  wristRotation.write(INIT_WRIST_ROTATE); // Wrist Motor range: 15 (out) to 180 (in)   and 145 is neutral
  delay(1000);
  wristSide2Side.write(INIT_WRIST_SIDE2SIDE); // Carpel Motor range : 0 (in) t0 90 (out) 
  delay(1000);
 

 /************************Motor**********************************************/  
  initialPotVal = analogRead(A6);
  Wire.beginTransmission(DEVICE_ADDRESS); //connecting to slave i.e DC motor controller
  Wire.write(M1_MODE_REGISTER); //Writing to mode register, i.e power mode
  Wire.write(0x00); 
  Wire.endTransmission(0);
  delay(1000);

  Wire.beginTransmission(DEVICE_ADDRESS); //connecting to slave i.e DC motor controller
  Wire.write(M2_MODE_REGISTER); //Writing to mode register, i.e power mode
  Wire.write(0x00); 
  Wire.endTransmission(0);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
 nh.spinOnce();
 delay(1);
}

void moveToFinalPos() //moving upperarm to lower position 
{
 
  potVal = analogRead(A6);
  while(abs(potVal-initialPotVal)<100)
  {
  delay(1000);
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(-30);
  Wire.endTransmission(0); 
  delay(100);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(0);
  Wire.endTransmission(0); 
  potVal = analogRead(A6);
  }

 fingers.write(75);
 delay(2000);
 
 for(int m2count=0;m2count<3;m2count++) //moving shoulder forward
 {
  delay(1000);
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M2_POWER_REGISTER); 
  Wire.write(40);
  Wire.endTransmission(0); 
  delay(100);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M2_POWER_REGISTER); 
  Wire.write(0);
  Wire.endTransmission(0); 
 }

 //servo code
 wristUpDown.write(110); //making wrist straight
 delay(2000);
 fingers.write(123);  //closing the fingers
 delay(2000);

 for(int m2count=0;m2count<2;m2count++)  //bringing shoulder back
 {
  delay(1000);
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M2_POWER_REGISTER); 
  Wire.write(-40);
  Wire.endTransmission(0); 
  delay(100);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M2_POWER_REGISTER); 
  Wire.write(0);
  Wire.endTransmission(0); 
 }
  
}
void moveToInitialPos() //moving upper arm to upper position
{
  potVal = analogRead(A6);
  while(potVal<initialPotVal-20)
  {
  delay(1000);
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(100);
  Wire.endTransmission(0); 
  delay(100);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(M1_POWER_REGISTER); 
  Wire.write(0);
  Wire.endTransmission(0); 
  potVal = analogRead(A6);
  }
  delay(2000);
  wristUpDown.write(75);
  iteration = false;
}


