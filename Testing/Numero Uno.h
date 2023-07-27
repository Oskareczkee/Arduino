#include <Arduino.h>
#include <time.h>

namespace Uno
{
void UnoSetup()
{
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(3, LOW);
  srand(time(NULL));
}

void LampsLoop()
{
  
  digitalWrite(8,HIGH);
  delay(1000);
  digitalWrite(8,LOW);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9,LOW);
}

void TurnOnButtonClick()
{
  if(digitalRead(2)==LOW)
  {
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
  }
  else
  {
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
  }
}

int GenerateRandomInt(unsigned int left, unsigned int right)
{
  return rand()%(right-left)+left;
}

//3 GREEN
//8 RED
//9 YELLOW
void TrafficLights()
{
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
    digitalWrite(3,LOW);
    delay(1000);

    while(digitalRead(2)==HIGH){}

    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
    digitalWrite(3,LOW);

    delay(2000);

    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(3,HIGH);

    //20 secs
    delay(20*1000);
}

void RandomLights()
{
  int availablePorts[]={8,9,3};

  int randomPort=GenerateRandomInt(0,3);

  for(int x=0;x<3;x++)
    digitalWrite(availablePorts[x], LOW);
  
  digitalWrite(availablePorts[randomPort], HIGH);
  delay(100);
}

void LightsWave()
{
    int availablePorts[]={8,9,3};

//wave to right , y=1 to not double red light
for(int y=1;y<3;y++)
{
  for(int x=0;x<3;x++)
    digitalWrite(availablePorts[x], LOW);
  digitalWrite(availablePorts[y], HIGH);
  delay(300);
}

//wave to left y=1, to not double green light
for(int y=1;y>=0;y--)
{
  for(int x=0;x<3;x++)
    digitalWrite(availablePorts[x], LOW);
  digitalWrite(availablePorts[y], HIGH);
  delay(300);
}
}

void PinkTest()
{
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
}
}