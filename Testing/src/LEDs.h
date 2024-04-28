#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "LEDMorseCode.h"

namespace LEDs
{
    #define PORT 7

    #define PHOTORESISTOR_ANALOG A5
    #define POTENTIOMETR_ANALOG A0

    #define BUTTON 13
    #define BUTTON_LAMP 8

    #define MODENUMBER 6

    #define LAMP_OFF 2
    #define LAMP_ONE 3
    #define LAMP_TWO 4
    #define LAMP_THREE 5
    #define LAMP_LAST 6


    class LED
    {
        public:
        //modes are the same as ports
        enum class Mode{OFF=2, ONE=3 , TWO=4, THREE=5, LAST=6};

        private:
            unsigned int leds_num;
            CRGB* leds;
            Mode mode = Mode::ONE;
            uint8_t brightness=255;

            String morseMessage="SOS";
            float morseCodeTimeUnit = 0.2f;
            CRGB lightsColor = CRGB{152, 38, 240};

            CRGB LinearInterpolation(const CRGB& color1, const CRGB& color2, const float& percent)
            {
                double R = color1.red + percent * (color2.red - color1.red);
                double G = color1.green + percent * (color2.green - color1.green);
                double B = color1.blue + percent * (color2.blue - color1.blue);

                return CRGB{R,G,B};
            }

            //morse code

            void wait(const unsigned int& time_units)
            {
                //time unit is in seconds, to convert it to the ms we multiply by 1000
                for(unsigned int x=0;x<time_units;x++)
                    delay(morseCodeTimeUnit*1000);
            }

            void send_dash()
            {
                for(int x=0;x<3;x++)
                {
                    this->turnOff();
                    wait(3);
                    this->showColor(CRGB{255,0,0});
                }
            }

            void send_dot()
            {
                for(int x=0;x<3;x++)
                {
                    this->turnOff();
                    wait(3);
                    this->showColor(CRGB{255,0,0});
                }
            }

            //serial can hold max 64 bytes of data(8 characters)
            //commands should not be longer than 8 chars then
            void EvaluateCommand(String& command)
            {
                command.trim();
                Serial.println(command);

                if(command=="exit")
                {
                    Serial.println("Exiting...");
                    Mode lastMode = getMode();
                    changeMode();
                    changeModeLamp(lastMode, getMode());
                    return;
                }

                else if(command=="light")
                {   
                    Serial.println("light val: ");
                    int val = getInput().toInt();
                    setBrightness(val);
                    Serial.print("Light: ");
                    Serial.println(val);
                    return;
                }

                else if(command=="color")
                {
                    int R,G,B;
                    Serial.println("R: ");
                    R=getInput().toInt();
                    Serial.println("G: ");
                    G=getInput().toInt();
                    Serial.println("B: ");
                    B=getInput().toInt();
                    Serial.print("Color: ");
                    Serial.println(R);
                    Serial.println(G);
                    Serial.println(B);

                    lightsColor=CRGB{R,G,B};
                }
                           
                else if(command=="mode")
                {
                    Serial.println("Mode:");
                    int val = getInput().toInt();

                    Mode lastMode = getMode();

                    if(changeModeInt(val))
                    {
                       Serial.println(val);
                       changeModeLamp(lastMode, getMode());
                    }
                    else
                        Serial.println("Wrong value");

                    return;
                }
                else if(command=="morse")
                {
                    Serial.println("message: ");
                    String mess = getInput();
                    morseMessage=mess;
                    Serial.println(mess);

                    return;
                }              
                else
                    Serial.println("Wrong command");
            }

            String getInput()
            {
                while(Serial.available()==0){
                    if(isButtonClicked())
                    {
                        changeMode();             
                        return;
                    }
                }
                String input = Serial.readStringUntil('\n');
                return input;
            }  


        public:

            LED(const unsigned int& _port, const unsigned int& _led_num): leds(new CRGB[_led_num]), leds_num(_led_num){}
            void setup()
            {
                Serial.begin(9600);
                Serial.setTimeout(120*1000);

                pinMode(PORT, OUTPUT);

                pinMode(BUTTON_LAMP, OUTPUT);
                pinMode(LAMP_OFF, OUTPUT);
                pinMode(LAMP_ONE, OUTPUT);
                pinMode(LAMP_TWO, OUTPUT);
                pinMode(LAMP_THREE, OUTPUT);
                pinMode(LAMP_LAST, OUTPUT);

                digitalWrite(BUTTON_LAMP, LOW);

                //turn off all mode lamps
                for(int x=2;x<=6;x++)
                    digitalWrite(x, LOW);
                //turn on only acutal mode lamp
                digitalWrite((int)mode, HIGH);

                pinMode(BUTTON, INPUT_PULLUP);

                FastLED.addLeds<WS2812, PORT, GRB>(leds, leds_num);
            }

            ~LED(){delete[] leds;}

            int getPhotoresistorRead()
            {
                return analogRead(PHOTORESISTOR_ANALOG);
            }

            //return values are mapped in range 0-255
            int getPotentiometrRead()
            {
                return map(analogRead(POTENTIOMETR_ANALOG), 0, 1023, 0, 255);
            }

            void setBrightness()
            {
                brightness = getPotentiometrRead();
                FastLED.setBrightness(brightness);
            }

            void setBrightness(float val)
            {
                brightness = val;
                FastLED.setBrightness(brightness);
            }

            uint8_t getBrightness(){return brightness;}

            bool isButtonClicked()
            {
                return digitalRead(BUTTON)==LOW ? true : false;
            }

            void turnOff()
            {
                FastLED.clear();
                FastLED.show();
            }

            void changeMode()
            {
                Mode lastMode = mode;

                if(mode==Mode::LAST)
                    mode=Mode::OFF;
                else
                    mode=(Mode)((int)mode+1);

                changeModeLamp(lastMode, mode);   
            }


            //returns true if was successfull        
            bool changeModeInt(unsigned int val)
            {
                if(val>MODENUMBER)
                    return false;
                //there is 2 offset casued byt lamp pins
                mode=(Mode)((int)(val+2));
                return true;
            }

            Mode getMode()
            {
                return mode;
            }

            void lightButtonLamp(const int& ms=1000)
            {
                digitalWrite(BUTTON_LAMP, HIGH);
                delay(ms);
                digitalWrite(BUTTON_LAMP, LOW);
            }

            void changeModeLamp(const Mode& lastMode, const Mode& newMode)
            {
                digitalWrite((int)lastMode, LOW);
                digitalWrite((int)newMode, HIGH);
            }


        //this saves actually a lot of memory, so try not to use debug
        #ifdef DEBUG
            void printInfoToSerial()
            {
                Serial.print("Photoresistor value: ");  Serial.println(getPhotoresistorRead());
                Serial.print("Potentiometr value: "); Serial.println(getPotentiometrRead());
                Serial.print("Button clicked: ");
                if(digitalRead(BUTTON)==LOW)
                    Serial.println("true");
                else
                    Serial.println("false");
                Serial.print("Mode: ");  Serial.println((int)mode);
                Serial.print("Brightness: "); Serial.println(brightness);
                Serial.print("Number of leds: "); Serial.println(leds_num);
            }
        #endif
            
            //show color, but does not change colors on all leds
            void showColor(const CRGB& color)
            {
                for(int i=0;i<leds_num;i++)
                    leds[i]=color;
                FastLED.show();
            }

            void test()
            {
                for (int i = 0; i < leds_num; i++)
                {
                    leds[i]=CRGB(255,255,153);
                    FastLED.show();
                    delay(5);
                }
                for (int i = leds_num ; i >=0; i--)
                {
                    leds[i]=CRGB(0, 255, 0);
                    FastLED.show();
                    delay(5);
                }
            }

            void consoleMode()
            {
                Serial.println("command: ");
                String command = getInput();
                EvaluateCommand(command);
            }

            void police()
            {
                FastLED.showColor(CRGB{255,0,0});
                delay(1000);
                FastLED.showColor(CRGB{0,0,255});
                delay(1000);
            }

            void color()
            {
                FastLED.showColor(lightsColor);
                //to reduce blinking
                delay(1000);
            }

            void gradient()
            {
                CRGB from{198, 15, 15};
                CRGB to{53, 1, 89};

                for(int x=0;x<(leds_num/2);x++)
                {
                    float percent = x/((float)(leds_num/2));

                    leds[x]=LinearInterpolation(from, to, percent);
                    leds[(leds_num-1)-x]=LinearInterpolation(from, to, percent);
                    delay(5);
                    FastLED.show();
                }
                for(int x=0;x<(leds_num/2);x++)
                {
                    float percent = x/((float)(leds_num/2));

                    leds[x]=LinearInterpolation(to, from, percent);
                    leds[(leds_num-1)-x]=LinearInterpolation(to, from, percent);
                    delay(5);
                    FastLED.show();
                }
            }

            void sendMorse(const String& message)
            {
                String encrypted_message = MorseCode::encrypt(message);

                for(const char& c: encrypted_message)
                {
                    switch (c)
                    {
                    case '-':
                        send_dash();
                        break;
                    case '.':
                        send_dot();
                        break;
                    case ' ':
                        wait(3);
                        break;
                    case '/':
                        wait(7);
                        break;
                    default:
                        Serial.println("undefined symbol");
                        return;
                    }
                }
            }

            void checkStates()
            {
                if(isButtonClicked())
                {
                    LED::Mode lastMode = getMode();
                    lightButtonLamp(3000);
                    changeMode();
                }
                setBrightness();
            }

            void loop()
            {
                checkStates();
                if(getPhotoresistorRead()<500)
                {
                    switch (getMode())
                    {
                        case LED::Mode::OFF:
                            turnOff();
                            break;
                        case LED::Mode::ONE:
                            color();
                            break;
                        case LED::Mode::TWO:
                            gradient();
                            break;
                        case LED::Mode::THREE:
                            sendMorse(morseMessage);
                            break;
                        case LED::Mode::LAST:
                            consoleMode();
                            break;                 
                        default:
                            turnOff();
                            #ifdef DEBUG
                                printInfoToSerial();
                            #endif
                            break;
                    }
                }
                else
                     turnOff();
            }
    };
}


