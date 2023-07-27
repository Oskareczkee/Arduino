#pragma once
#include <Arduino.h>
#include "LEDs.h"
//#include <string.h>


class LEDMorseCode
{
    public:
    //encrypts to morse code format (dots and dashes)
    String encrypt(const String& message);
    void send(const String& encrypted_message);

    void set_time_unit(const float& unit);

    private:
    
    LEDs::LED& led;

    static const String Alphabet[26];
    float time_unit=0.1f;
    void send_dash();
    void send_dot();
    void wait(const unsigned int& time_units);

    public:
    //constructors
    LEDMorseCode(const LEDs::LED& leds, const float& _time_unit): led(leds), time_unit(_time_unit){}
};

//alphabet is in alphabetical order
const String LEDMorseCode::Alphabet[26]={
        ".-",
        "-...",
        "-.-.",
        "-..",
        ".",
        "..-.",
        "--.",
        "....",
        "..",
        ".---",
        "-.-",
        ".-..",
        "--",
        "-.",
        "---",
        ".--.",
        "--.-",
        ".-.",
        "...",
        "-",
        "..-",
        "...-",
        ".--",
        "-..-",
        "-.--",
        "--.."
    };

String LEDMorseCode::encrypt(const String& message)
{
    String output="";
    for(const char& c: message)
    {
        if(c==' ')
            output+='/';
        else
        {
            output+=Alphabet[tolower(c)-97];
            output+=" ";
        }
    }
    return output;
}

void LEDMorseCode::set_time_unit(const float& unit)
{
    time_unit=unit;
}

void LEDMorseCode::wait(const unsigned int& time_units)
{
    //time unit is in seconds, to convert it to the ms we multiply by 1000
    for(unsigned int x=0;x<time_units;x++)
        delay(time_unit*1000);
}

void LEDMorseCode::send_dash()
{
    for(int x=0;x<3;x++)
    {
        led.showColor(CRGB{255,0,0});
        wait(3);
        led.showColor(CRGB{0,0,255});
    }
}

void LEDMorseCode::send_dot()
{
    for(int x=0;x<3;x++)
    {
        led.showColor(CRGB{255,0,0});
        wait(1);
        led.showColor(CRGB{0,0,255});
    }
}

//1 time unit between dots and dashes
//3 time units between letters
//7 time units when the word is completed
void LEDMorseCode::send(const String& encrypted_message)
{
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
            Serial.println("LEDMorseCode::send: undefined symbol!");
            return;
        }
    }
}