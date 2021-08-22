/*  
 *  Senior Design Project - Occular VR Headset
 *  Derivative Method
 */

#include <Mouse.h>

const int xAxis = A0;           //Pin A0 - xAxis
const int yAxis = A1;           //Pin A1 - yAxis

int range = 12;                 // Range for x and y axis after mapping
int center = range/2;           // Center of x & y axis after mapping
int eyeThreshold = range/4;     // Threshold to stop micro-movements after mapping
int minimum[] = {1023, 1023};   // Maximum value
int maximum[] = {0, 0};         // Minimum value
int axis[] = {xAxis, yAxis};    // x and y axis array
int responseDelay = 500;        // Delay to give time for the mouse to responde

float moveMouse()
{

  int x = readAxis(0);
  int y = readAxis(1);
  
  Mouse.move(x, y, 0);    // Mouse.move(x axis, y axis, scroll wheel)
  delay(responseDelay);
}

int readAxis(int axisNumber)
{
  int distance = 0;

  int value = analogRead(axis[axisNumber]);

  if (value < minimum[axisNumber])
    minimum[axisNumber] = value;

  if (value > maximum[axisNumber])
    maximum[axisNumber] = value;
    
  /* map(valueToMap, currentMin, currentMax, desiredMin, desiredMax) 
    Takes a value and the values that it is bound by, and bounds them to another set of values.*/
  value = map(value, minimum[axisNumber], maximum[axisNumber], 0, range);

  if (abs(value - center) > eyeThreshold)
    distance = (value - center);

  if (axisNumber == 1)
    distance = -distance;

  if (axisNumber == 0)
  {
    Serial.println("----------------------------------------------------------------------");
    Serial.print("X Distance = ");
    Serial.println(distance);
    Serial.print("X Value = ");
    Serial.println(value);
  }

  else
  {
    Serial.print("Y Distance = ");
    Serial.println(distance);
    Serial.print("Y Value = ");
    Serial.println(value);
  }

  return distance;
  
}

void setup() 
{
  Serial.begin(9600);
  Mouse.begin();
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);

}

void loop()
{
  moveMouse();
}
