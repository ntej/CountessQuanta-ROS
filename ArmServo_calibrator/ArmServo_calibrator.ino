

/* Program was design to controll 3 servos
and using serial instructions by sending first 
a character followed by a number. 
The leter inidicates the uniques servo and the 
number the command to be sent*/
 
#include <Servo.h>  // arduino servo library
#define spoon_delay                 10      // delay for spoon stepsc 
#define  FINGER_SERVO_POS           22      //'F'
#define  WRISTS2S_SERVO_POS         24      //'C'
#define  WRISTUPDOWN_SERVO_POS      26      //'A'
#define  WRISTROTATION_SERVO_POS    30      //'W' rc=

#define INIT_FINGERS 0 
#define INIT_WRIST_UD 60
#define INIT_WRIST_SIDE2SIDE 60
#define INIT_WRIST_ROTATE 60

//#define Silent    // use for debug only
// instatiate a servos
Servo fingers;
Servo wristRotation;
Servo wristSide2Side;
Servo wristUpDown;

int presentFState;
int presentAState;
int presentCState;
int presentWState;

void setup() {
  // initialize serial communication at 9600 baudrate 
  Serial.begin(9600); 
   
    fingers.attach(FINGER_SERVO_POS);
    wristRotation.attach(WRISTROTATION_SERVO_POS);
    wristSide2Side.attach(WRISTS2S_SERVO_POS);
    wristUpDown.attach(WRISTUPDOWN_SERVO_POS);
    
   
    delay (200); 

    //Initialization
    wristUpDown.write(INIT_WRIST_UD); 
    
     delay(1000);
    fingers.write(INIT_FINGERS);       // Finger Motor range: 90 (close) to 180 (open)
    delay(1000);
    wristRotation.write(INIT_WRIST_ROTATE);         // Wrist Motor range: 15 (out) to 180 (in)   and 145 is neutral
     delay(1000);
    wristSide2Side.write(INIT_WRIST_SIDE2SIDE);        // Carpel Motor range : 0 (in) t0 90 (out) 
     delay(1000);
     
     
     presentFState = INIT_FINGERS;
     presentAState = INIT_WRIST_UD;
     presentCState = INIT_WRIST_SIDE2SIDE;
     presentWState = INIT_WRIST_ROTATE;  
    
}

void loop() {

  int pos, spoon_pos; // variables to be used to send commands

  int currentServoPosition;               // This keeps the information of servo position
  int lastServoPosition;                  // Last Servo position
  
  // check serial buffer has something 
  if (Serial.available() > 0) {
    // read first byte on serial and store it f= 90
    
    int inByte = Serial.read();

    // check if it is one the defined cases
    switch (inByte) {
      
    //=======================================================================
    case 'f':    
        // Valid case, wait for an int and store it in pos
        spoon_pos = Serial.parseInt();
       
        // provides feedback for debuf if needed
        #ifndef Silent
        Serial.print("Servo Fingers=");
        Serial.println(pos);
        #endif /* Silent */
            // only two aceptables values for spoon 180 and 0
      if (spoon_pos<=presentFState)
      {
        // loop from 0-90 using steps of 2 has a defined delay
        for (int i=presentFState; i>=spoon_pos; i=i-2)
        {
          fingers.write(i);
         delay(spoon_delay);
        }  
        presentFState = spoon_pos;      
      }
       else if( spoon_pos>=presentFState){
        // loop from 90 back to 0 with steps of 2 has a defined delay
         for (int i=presentFState; i <=spoon_pos; i=i+2)
        {
          fingers.write(i);
         delay(spoon_delay);
        }
        presentFState = spoon_pos;
       }
       
       else
         fingers.write(spoon_pos);
      break;
      //====================================================================
    case 'a':    
        // Valid case, wait for an int and store it in pos
       spoon_pos = Serial.parseInt();
       
        // provides feedback for debuf if needed
        #ifndef Silent
        Serial.print("WristUpDown position=");
        Serial.println(pos);
        #endif /* Silent */
            // only two aceptables values for spoon 180 and 0
      if ( spoon_pos <=presentAState)
      {
        // loop from 0-90 using steps of 2 has a defined delay
        for (int i=presentAState; i>=spoon_pos; i=i-1)
        {
          wristUpDown.write(i);
         delay(spoon_delay);
        }  
        presentAState = spoon_pos;      
      }
       else if( spoon_pos >=presentAState){
        // loop from 90 back to 0 with steps of 2 has a defined delay
         for (int i=presentAState; i <= spoon_pos; i=i+1)
        {
          wristUpDown.write(i);
         delay(spoon_delay);
        }
        presentAState = spoon_pos;
       }
      break;
      //=====================================================================
    case 'w':    
        spoon_pos = Serial.parseInt();
        
        #ifndef Silent  
        Serial.print("Servo wrist_position=");
        Serial.println(pos);
        #endif /* Silent */
            // only two aceptables values for spoon 180 and 0
      if (spoon_pos <=presentWState)
      {
        // loop from 0-90 using steps of 2 has a defined delay
        for (int i=presentWState; i>=spoon_pos; i=i-2)
        {
          wristRotation.write(i);
         delay(spoon_delay);
        }     
       presentWState = spoon_pos;   
      }
       else if( spoon_pos >=presentWState){
        // loop from 90 back to 0 with steps of 2 has a defined delay
         for (int i=presentWState; i <=spoon_pos; i=i+2)
        {
          wristRotation.write(i);
         delay(spoon_delay);
        }
        presentWState = spoon_pos;
       }
      break;
      //=====================================================================

    case 'c':    
        spoon_pos = Serial.parseInt();
        
        #ifndef Silent  
        Serial.print(" wrist_side2side position=");
        Serial.println(pos);
        #endif /* Silent */
        
         
      // only two aceptables values for spoon 90 and 0
      if ( spoon_pos <=presentCState)
      {
        // loop from 0-90 using steps of 2 has a defined delay
        for (int i=presentCState; i>=spoon_pos; i=i-2)
        {
          wristSide2Side.write(i);
         delay(spoon_delay);
        }   
       presentCState = spoon_pos;     
      }
       else if( spoon_pos >=presentCState){
        // loop from 90 back to 0 with steps of 2 has a defined delay
         for (int i=presentCState; i <=spoon_pos; i=i+2)
        {
         wristSide2Side.write(i);
         delay(spoon_delay);
        }
        presentCState = spoon_pos;
       }
      break; 
//================================================================================   
   // if no case define it doesn't do anything
    default:
      break;
       
  }/* end switch*/
  }/* end if */
  }/* end while */

