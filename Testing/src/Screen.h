#include <Arduino.h>
#include "LiquidCrystal.h"

namespace ScreenTest
{ 
    LiquidCrystal lcd(2,3,4,5,6,7);
    #define BACKLIGHT_CONTROL 9
    #define PHOTOTESISTOR_ANALOG A0
    #define BUTTON 8

    void ScreenSetup()
    {
        Serial.begin(9600);
        lcd.begin(16, 2); //Type declaration
        lcd.setCursor(0, 0);
        analogWrite(BACKLIGHT_CONTROL,48);
        pinMode(8, INPUT_PULLUP);
    }

    void ShowPhotoresistorValue()
    {
        lcd.print("Nap. FotoRez");
        lcd.setCursor(0,1);
        lcd.print(analogRead(PHOTOTESISTOR_ANALOG)*(5.0/1024), 4);
        lcd.print("V");
        delay(300);
        lcd.clear();
    }

    void ButtonCheck()
    {
        if(digitalRead(BUTTON)==LOW)
        {
            Serial.print("Clicked !");
            delay(50);
        }
    }

    void Stoper()
    {
        lcd.clear();
        lcd.setCursor(0,0);
        if(digitalRead(BUTTON)==LOW)
        {
            lcd.print("Stoper Start");
            for(int x=0;x<3;x++)
            {
                lcd.print(".");
                delay(400);
            }
            unsigned long time1=millis();
            lcd.clear();
            lcd.print("Click button");
            lcd.setCursor(0,1);
            lcd.print("To end");

            while(digitalRead(BUTTON)==HIGH){}

            unsigned long time2 = millis();

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Time expired: ");
            lcd.setCursor(0,1);
            lcd.print((time2-time1)/1000.0, 3);
            lcd.print("s");

            delay(500);

            while(digitalRead(BUTTON)==HIGH){}

        }
    }
}