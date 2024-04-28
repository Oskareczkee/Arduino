
#include <Arduino.h>

namespace Due
{
    #define RED 8
    #define YELLOW 9
    #define GREEN 3
    #define BUTTON 2

    constexpr int availablePort[]={8,9,3,2};

    void DosSetup()
    {
          pinMode(8,OUTPUT);
          pinMode(9,OUTPUT);
          pinMode(3, OUTPUT);
          pinMode(2, INPUT_PULLUP);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          digitalWrite(3, LOW);

          Serial.begin(9600);
          Serial.println("siema siema o tej porze każdy wypić może");
          Serial.println("O mammale");
          Serial.setTimeout(120*1000);
    }

    void resetLights()
    {
        digitalWrite(RED, LOW);
        digitalWrite(YELLOW, LOW);
        digitalWrite(GREEN, LOW);
    }

    void switchLights()
    {
        //digitalWrite(RED, HIGH);
        if(Serial.available())
        {
            String receivedData="";
            receivedData=Serial.readStringUntil('\n');
            Serial.read();
  
            Serial.println(receivedData);

            if(receivedData=="red")
            {
                resetLights();
                digitalWrite(RED, HIGH);
            }
            else if(receivedData=="green")
            {
                resetLights();
                digitalWrite(GREEN, HIGH);
            }
            else if(receivedData=="yellow")
            {
                resetLights();
                digitalWrite(YELLOW, HIGH);
            }
            else
            {
                Serial.println("Wrong color!");
            }
        }
    }

    void switchLights2()
    {
        //digitalWrite(RED, HIGH);
        if(Serial.available())
        {
            String receivedData="";
            receivedData=Serial.readStringUntil('\n');
  
            Serial.println(receivedData);

            if(receivedData=="red")
            {
                if(digitalRead(RED)==HIGH)
                    digitalWrite(RED, LOW);
                else
                    digitalWrite(RED, HIGH);
            }
            else if(receivedData=="green")
            {
                if(digitalRead(GREEN)==HIGH)
                    digitalWrite(GREEN, LOW);
                else
                    digitalWrite(GREEN, HIGH);
            }
            else if(receivedData=="yellow")
            {
                if(digitalRead(YELLOW)==HIGH)
                    digitalWrite(YELLOW, LOW);
                else
                    digitalWrite(YELLOW, HIGH);
            }
            else
            {
                Serial.println("Wrong color!");
            }
        }

    }
}