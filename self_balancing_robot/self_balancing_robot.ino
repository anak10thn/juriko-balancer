#include <Wire.h> //Include the Wire library

// wired connections
#define HG7881_B_IA 3 // D3 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 2 // D2 --> Motor B Input B --> MOTOR B -
#define HG7881_A_IA 5 // D5 --> Motor A Input A --> MOTOR A +
#define HG7881_A_IB 4 // D4 --> Motor A Input B --> MOTOR A -
 
// functional connections
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction
#define MOTOR_A_PWM HG7881_A_IA // Motor B PWM Speed
#define MOTOR_A_DIR HG7881_A_IB // Motor B Direction

/* Accelerometer Info
 http://yourduino.com/sunshop2/index.php?l=product_detail&p=391
 +5V and GND to Arduino/Yourduino (Module has 3.3V regulator)
 SCL -  A5 
 SDA -  A4
*/
#include <Wire.h> //Include the Wire library
#include <MMA_7455.h> //Include the MMA_7455 library    Accelerometer

MMA_7455 mySensor = MMA_7455(); //Make an instance of MMA_7455    Accelerometer
char xVal, yVal, zVal; //Variables for the values from the sensor  Accelerometer

int set = 0;
const float kp = 9;
const float kd = 7.75;
int error = 0;
int oldError = 0;
float sum = 0.0;
float pAction = 0.00;
float dAction = 0.00;
float Action = 0.00;
float time = 0;
float oldTime = 0;
int index=0;
int nAvg=4;
int de[4]= {0,0,0,0};
float deAvg=0.00;

void setup(){
  Serial.begin(115200);
  
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_A_PWM, OUTPUT);
  digitalWrite(MOTOR_B_DIR, LOW);
  digitalWrite(MOTOR_B_PWM, LOW);
  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_A_PWM, LOW);
  
  mySensor.initSensitivity(2); // Set the sensitivity you want to use: 2 = 2g (Good for "Tilt" measurements), 4 = 4g, 8 = 8g
  mySensor.calibrateOffset(10,17,-7);
  
}

void loop() 
{
  time = millis();
  
  xVal = mySensor.readAxis('x'); //Read out the 'x' Axis
  yVal = mySensor.readAxis('y'); //Read out the 'y' Axis
  zVal = mySensor.readAxis('z'); //Read out the 'z' Axis
  
  error = zVal - set;
  sum = sum - de[index];
  de[index] = (error-oldError)/((time-oldTime)/1000);  //moving average for dError
  sum = sum + de[index];
  deAvg = sum/nAvg;
  index++;
  if (index >= nAvg)
  {
    index = 0;
  }

pAction = kp*error;
dAction = kd*deAvg;
Action = (pAction + dAction);
Action = constrain(Action, -255, 255);
  
  if (Action > 0) {
    digitalWrite(MOTOR_B_DIR, HIGH); 
    analogWrite(MOTOR_B_PWM, 255 - Action);
    digitalWrite(MOTOR_A_DIR, LOW); 
    analogWrite(MOTOR_A_PWM, Action);
    Serial.println("lebih dari 0");
   }
   else if (Action < 0) {
     digitalWrite(MOTOR_B_DIR, LOW); 
     analogWrite(MOTOR_B_PWM, 0- Action);
     digitalWrite(MOTOR_A_DIR, HIGH); 
     analogWrite(MOTOR_A_PWM, 255 - (0-Action));
     Serial.println("kurang dari 0");
   }
   else {
     digitalWrite(MOTOR_B_DIR, LOW); 
     analogWrite(MOTOR_B_PWM, 0);
     digitalWrite(MOTOR_A_DIR, LOW); 
     analogWrite(MOTOR_A_PWM, 0);
     Serial.println("selebihnya");
   }
  
  Serial.print("Z = ");
  Serial.print(zVal, DEC);
  Serial.print("  Action: ");
  Serial.println(Action);

  delay(20);
}
