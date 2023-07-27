#include <Arduino.h>

namespace Sei
{
    #define SENSOR_TRIGGER 3 
    #define SENSOR_ECHO 2
    #define BUZZER 4

    #define LIGHT_1 9
    #define LIGHT_2 10
    #define LIGHT_3 11
    #define LIGHT_4 12

    void SeiSetup()
    {
        Serial.begin(9600);
        pinMode(SENSOR_TRIGGER, OUTPUT);
        pinMode(SENSOR_ECHO, INPUT);    
        pinMode(BUZZER, OUTPUT);
        digitalWrite(BUZZER, LOW);

        pinMode(LIGHT_1, OUTPUT);
        pinMode(LIGHT_2, OUTPUT);
        pinMode(LIGHT_3, OUTPUT);
        pinMode(LIGHT_4, OUTPUT);
    }

    float measureDistance()
    {
        digitalWrite(SENSOR_TRIGGER, LOW);
        delayMicroseconds(2);//make ready to measuere
        digitalWrite(SENSOR_TRIGGER, HIGH);
        delayMicroseconds(10);//measure
        digitalWrite(SENSOR_TRIGGER, LOW);

        long time = pulseIn(SENSOR_ECHO, HIGH);
        float distance = time/58.0;// distance is in cm, 58 microseconds it takes for sound to cover the distance of 1cm

        return distance;
    }

    void turnLightsOff()
    {
        digitalWrite(LIGHT_1, LOW);
        digitalWrite(LIGHT_2, LOW);
        digitalWrite(LIGHT_3, LOW);
        digitalWrite(LIGHT_4, LOW);
    }

    void TurnLightsDependingOnDistance()
    {
        long distance = measureDistance();
        int val = map(distance, 0, 400, 1, 4);//up to 4 meters

        Serial.print(distance);
        Serial.println(" cm");

        switch(val)
        {
            case 1:
                turnLightsOff();
                digitalWrite(LIGHT_1, HIGH);
                break;
            case 2:
                turnLightsOff();
                digitalWrite(LIGHT_2, HIGH);
                break;
            case 3:
                turnLightsOff();
                digitalWrite(LIGHT_3, HIGH);
                break;
            case 4:
                turnLightsOff();
                digitalWrite(LIGHT_4, HIGH);
                break;
            default:
                turnLightsOff();
                break;
        }
    }
}