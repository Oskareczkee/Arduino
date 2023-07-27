#include <Arduino.h>

namespace Tre
{
    //these defines can be changed in case you have lights connected to other pins

    #define RED 8
    #define YELLOW 9
    #define GREEN 3
    #define BUTTON 2
    #define BLUE 4
    #define PHOTORESISTOR_ANALOG A0
    #define PHOTORESISTOR2_ANALOG A3
    #define POTENTIOMETR_ANALOG A1

    constexpr int availablePort[]={8,9,3,2,4};

    void TresSetup()
    {
          pinMode(8,OUTPUT);
          pinMode(9,OUTPUT);
          pinMode(3, OUTPUT);
          pinMode(2, INPUT_PULLUP);
          pinMode(4, OUTPUT);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          digitalWrite(3, LOW);

          Serial.begin(9600);
          Serial.println("siema siema o tej porze każdy wypić może");
          Serial.println("O mammale");
          Serial.setTimeout(120*1000);
    }
    
    //resets all lights to LOW
    void ResetLights()
    {
          digitalWrite(RED, LOW);
          digitalWrite(YELLOW, LOW);
          digitalWrite(GREEN, LOW);
          digitalWrite(BLUE, LOW);
    }

void potentiometrTest()
{
    float value=analogRead(POTENTIOMETR_ANALOG);
    int scaledValue=map(value, 0, 1023,1,4);

    switch(scaledValue)
    {
        case 1:
            ResetLights();
            digitalWrite(GREEN, HIGH);
            break;
        case 2:
            ResetLights();
            digitalWrite(YELLOW, HIGH);
            break;
        case 3:
            ResetLights();
            digitalWrite(RED, HIGH);
            break;
        case 4:
            ResetLights();
            digitalWrite(BLUE, HIGH);
            break;
        default:
            Serial.println("Something went wrong!");
            break;
    }
    delay(50);
}

//requires photoresistor
void autoNightLamp()
{
    float value=analogRead(PHOTORESISTOR_ANALOG);
  if(value<50)
    digitalWrite(GREEN, HIGH);
  else
    digitalWrite(GREEN, LOW);
}


void LightsWave(unsigned int Delay)
{
  int availablePorts[]={8,9,3,4};

  //wave to right , y=1 to not double red light
  for(int y=1;y<4;y++)
  {
    ResetLights();
    digitalWrite(availablePorts[y], HIGH);
    delay(Delay);
  }

  //wave to left y=1, to not double green light
  for(int y=2;y>=0;y--)
  {
    ResetLights();
    digitalWrite(availablePorts[y], HIGH);
    delay(Delay);
  }
}


//requires potentiometr and photoresistor and light of course
void regulatedautoNightLamp()
{
  float value=analogRead(PHOTORESISTOR_ANALOG);
  float limit=analogRead(POTENTIOMETR_ANALOG);
  if(value<limit)
    LightsWave(100);
  else
    ResetLights();
}

void regulatedautoNightLampShowVoltage()
{
  float value=analogRead(PHOTORESISTOR_ANALOG);
  float limit=analogRead(POTENTIOMETR_ANALOG);
  float voltage=limit*(5.0f/1024);
  Serial.println(voltage);
  if(value<limit)
    digitalWrite(GREEN, HIGH);
  else
    digitalWrite(GREEN, LOW);

  delay(1000);
}

void ADCGame()
{
  int chances=3;
  int value=analogRead(POTENTIOMETR_ANALOG);
  Serial.println(value);
  while(chances--)
  {
    int guess=0;
    Serial.println("Type value of ADC on potentiometr");

    while(Serial.available()==0){}
      guess=Serial.parseInt();

    if((guess>(value+50)) || (guess<(value-50)))
    {
      digitalWrite(YELLOW, HIGH);
      Serial.print("Bruh, very bad, try once more\nYou have "); Serial.print(chances); Serial.print(" chances\n");
      delay(1500);
      ResetLights();
      continue;
    }
    else
    {
      digitalWrite(GREEN, HIGH);
      Serial.println("Very good, you won, reset arduino to play once more!");
      return;
    }
  }
    digitalWrite(RED, HIGH);
    Serial.println("Bruh, you lost");
}

void photoresistorsTest()
{
  Serial.print("Photoresistor 1 ");
  Serial.println(analogRead(PHOTORESISTOR_ANALOG));
  Serial.print("Photoresistor 2 ");
  Serial.println(analogRead(PHOTORESISTOR2_ANALOG));
  delay(2*1000);
}

}