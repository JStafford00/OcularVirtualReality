/*
    Senior Design Project - Occular VR Headset
    Derivative Method
*/

#include <Mouse.h>

#define WINDOW_SIZE 400         // window for moving average

const int xAxis = A1;           //Pin A1 - xAxis
const int yAxis = A3;           //Pin A3 - yAxis

int range = 12;                 // Range for x and y axis after mapping
int center = range / 2;         // Center of x & y axis after mapping
int eyeThreshold = range / 6;   // Threshold to stop micro-movements after mapping
int axis[] = {xAxis, yAxis};    // x and y axis array
int responseDelay = 20;         // Delay to give time for the mouse to responde

// JJ's solution
int analogRangeX = 150;         // (observed) range on analog read 
long movingSumX = 0;            // moving sum on analog read
int movingAverageX = 0;         // moving average on analog read
int indexX = 0;                 // index into circular array
int circularArrayX[WINDOW_SIZE];// circular array X

int analogRangeY = 100;         // (observed) range on analog read 
long movingSumY = 0;            // moving sum on analog read
int movingAverageY = 0;         // moving average on analog read
int indexY = 0;                 // index into circular array
int circularArrayY[WINDOW_SIZE];// circular array Y

int delayCounterY = 0;
bool enableMouseY = false;
int delayCounterThreshold = 5;
int distancePrevY = 0;

unsigned long start = millis();
unsigned long finish = 0;


float moveMouse()
{
  finish = millis();
  
  if (finish >= start + responseDelay)
  {
    int x = readAxis(0);
    int y = readAxis(1);
    Mouse.move(x, y, 0);    // Mouse.move(x axis, y axis, scroll wheel)
    start = millis();
  }
  
}

int readAxis(int axisNumber)
{
  int distance;

  int value = analogRead(axis[axisNumber]);

  // Upper and lower bounds on anolog read dictated by moving average
  int lowerBound = 0;
  int upperBound = 0;
  
  // put value into circular array and progress moving average
  // assign values for bounds
  if(axisNumber == 0)
  {
    distance = 0;
    movingSumX = movingSumX - circularArrayX[indexX];
    circularArrayX[indexX] = value;
    movingSumX = movingSumX + value;
    indexX = (indexX + 1)%WINDOW_SIZE;
    movingAverageX = movingSumX/WINDOW_SIZE;

    lowerBound = movingAverageX - analogRangeX/2;
    upperBound = movingAverageX + analogRangeX/2;
  }
  else
  {
    distance = 0;
    movingSumY = movingSumY - circularArrayY[indexY];
    circularArrayY[indexY] = value;
    movingSumY = movingSumY + value;
    indexY = (indexY + 1)%WINDOW_SIZE;
    movingAverageY = movingSumY/WINDOW_SIZE;

    lowerBound = movingAverageY - analogRangeY/2;
    upperBound = movingAverageY + analogRangeY/2;
  }

  /* map(valueToMap, currentMin, currentMax, desiredMin, desiredMax)
    Takes a value and the values that it is bound by, and bounds them to another set of values.*/
  int mappedValue = map(value, lowerBound, upperBound, 0, range);

  if (abs(mappedValue - center) > eyeThreshold){
    distance = (mappedValue - center);
    if(axisNumber==1){
      if(distance*distancePrevY>0){
        delayCounterY++;
      }
      else{
        delayCounterY=0;
      }
      distancePrevY=distance;
    }
  }
  else{
    if(axisNumber==1){
      delayCounterY = 0;
      distancePrevY=0;
    }
  }
  
  if(axisNumber==1){
    if(delayCounterY>=delayCounterThreshold){
      enableMouseY=true;
    }
    else{
      enableMouseY=false;
    }
  }
//  if (axisNumber == 1)
//    distance = -distance;

  if (axisNumber == 0)
  {
    Serial.println("----------------------------------------------------------------------");
    Serial.print("X value = ");
    Serial.println(value);
    Serial.print("X lowerBound = ");
    Serial.println(lowerBound);
    Serial.print("X upperBound = ");
    Serial.println(upperBound);
    Serial.print("X mappedValue = ");
    Serial.println(mappedValue);
    Serial.print("X Distance = ");
    Serial.println(distance);
  }

  else
  {
    Serial.print("Y value = ");
    Serial.println(value);
    Serial.print("Y lowerBound = ");
    Serial.println(lowerBound);
    Serial.print("Y upperBound = ");
    Serial.println(upperBound);
    Serial.print("Y mappedValue = ");
    Serial.println(mappedValue);
    Serial.print("Y Distance = ");
    Serial.println(distance);
  }

  if(axisNumber==1){
    if(enableMouseY){
      return distance;
    }
    else{
      return 0;
    }
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
