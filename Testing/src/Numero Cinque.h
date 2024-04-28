#include <Arduino.h>

namespace Cinque
{
    #define ENGINE_1_PWM 6
    #define ENGINE_1_DIRECTION_RIGHT 7
    #define ENGINE_1_DIRECTION_LEFT 8

    #define ENGINE_2_PWM 5
    #define ENGINE_2_DIRECTION_RIGHT 4
    #define ENGINE_2_DIRECTION_LEFT 2

    void CinqueSetup()
    {
          pinMode(ENGINE_1_PWM, OUTPUT);
          pinMode(ENGINE_2_DIRECTION_RIGHT, OUTPUT);
          pinMode(ENGINE_1_DIRECTION_LEFT, OUTPUT);

          pinMode(ENGINE_2_PWM, OUTPUT);
          pinMode(ENGINE_2_DIRECTION_RIGHT, OUTPUT);
          pinMode(ENGINE_2_DIRECTION_LEFT, OUTPUT);
    }

    void SimulateRotation()
    {
        //ENGINE 1
        analogWrite(ENGINE_1_PWM, 100);
        digitalWrite(ENGINE_1_DIRECTION_RIGHT, LOW);
        digitalWrite(ENGINE_1_DIRECTION_LEFT, HIGH);
        delay(1500);

        analogWrite(ENGINE_1_PWM, 255);//max left
        digitalWrite(ENGINE_1_DIRECTION_LEFT, LOW);
        digitalWrite(ENGINE_1_DIRECTION_RIGHT, HIGH);
        delay(1500);

        //ENGINE 2
        analogWrite(ENGINE_2_PWM, 100);
        digitalWrite(ENGINE_2_DIRECTION_RIGHT, LOW);
        digitalWrite(ENGINE_2_DIRECTION_LEFT, HIGH);
        delay(1500);

        analogWrite(ENGINE_2_PWM, 255);//max left
        digitalWrite(ENGINE_2_DIRECTION_LEFT, LOW);
        digitalWrite(ENGINE_2_DIRECTION_RIGHT, HIGH);
        delay(1500);
    }

    //smoothly accelerates and deaccelerates engine
    void accelerateEngine()
    {
        analogWrite(ENGINE_1_PWM, 0);
        digitalWrite(ENGINE_1_DIRECTION_LEFT, HIGH);
        int change=1;
        for(unsigned char x=1;x<254;x+=change)
        {
            analogWrite(ENGINE_1_PWM, x);
            delay(50);
        }
        for(unsigned char x = 254; x>0;x-=change)
        {
            analogWrite(ENGINE_1_PWM, x);
            delay(50);
        }

    }
}