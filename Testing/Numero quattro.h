#include <Arduino.h>
#include <Servo.h>

//#define BASIC_SETUP
#define SERVO_SETUP
namespace Quattro
{


#ifdef BASIC_SETUP
    #define RED 8
    #define YELLOW 9
    #define GREEN 3
    #define BUTTON 2
    #define BLUE 4
    #define PHOTORESISTOR_ANALOG A0
    #define PHOTORESISTOR2_ANALOG A3
    #define POTENTIOMETR_ANALOG A1
    #define GREEN_PWM 3 // Blue light is connected to pwm as well
    constexpr int availablePort[]={8,9,3,2,4};
#endif

#ifdef SERVO_SETUP
    #define SERVO_PIN 9
    #define POTENTIOMETR_ANALOG A1
    #define PHOTORESISTOR_ANALOG A0
#endif


#ifdef SERVO_SETUP
    Servo servo;
    void QuattroServoSetup()
    {
        Serial.begin(9600);
        Serial.setTimeout(120*1000);
        servo.attach(SERVO_PIN);
    }

    void moveServo()
    {
        int position=0;
        const int change=1;

        while(position<=180)
        {
            servo.write(position);
            position+=change;
            delay(15);
        }
    }

    void moveServoWithPotentiometr()
    {
        int value=map(analogRead(POTENTIOMETR_ANALOG),0,1023,0,180);
        servo.write(value);
        delay(50);
    }

    void printTest()
    {
        int valueToPrint = 121;

        Serial.print(valueToPrint, DEC);
        Serial.print("[DEC]\t");
        Serial.print(valueToPrint, HEX);
        Serial.print("[HEX] \t");
        Serial.print(valueToPrint, OCT);
        Serial.print("[OCT]\t");
        Serial.print(valueToPrint, BIN);
        Serial.print("[BIN]\n");
    }

    //i measured max light - 1000 (used my phone flashlight)
    void lightIndicator()
    {
        Serial.println(analogRead(PHOTORESISTOR_ANALOG));
        int servoAngle=map(analogRead(PHOTORESISTOR_ANALOG),0,1000,0,180);
        servo.write(servoAngle);
        delay(300);
    }

#endif

#ifdef BASIC_SETUP
    void QuattroSetup()
    {
          pinMode(8,OUTPUT);
          pinMode(9,OUTPUT);
          pinMode(3, OUTPUT);
          pinMode(2, INPUT_PULLUP);
          pinMode(4, OUTPUT);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);

          Serial.begin(9600);
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

    void brightenLight()
    {
        const int change = 1;
        unsigned char value=0;
        analogWrite(GREEN_PWM, value);

        while(value+=change)
        {
            analogWrite(GREEN_PWM,value);
            delay(100);
        }
    }
#endif
}