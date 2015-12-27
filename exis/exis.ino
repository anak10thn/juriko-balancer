#include <Wire.h> //Include the Wire library
#include <MMA_7455.h> //Include the MMA_7455 library
/*
in2 = 9
SDA = A4
SCL = A5
*/
MMA_7455 mySensor = MMA_7455();

char xVal, yVal, zVal;

void setup()
{
Serial.begin(9600);
delay(1500);
mySensor.initSensitivity(2);
}

void loop()
{
xVal = mySensor.readAxis('x'); //Read the ‘x’ Axis
yVal = mySensor.readAxis('y'); //Read the ‘y’ Axis
zVal = mySensor.readAxis('z'); //Read the ‘z’ Axis
Serial.print("X = ");
Serial.print(xVal, DEC);
Serial.print(" Y = ");
Serial.print(yVal, DEC);
Serial.print(" Z = ");
Serial.println(zVal, DEC);
delay(1000);
}
