#pragma once
#include <Arduino.h>

class MorseCode
{
    public:
    //encrypts to morse code format (dots and dashes)
    static String encrypt(const String& message);
    void send(const String& encrypted_message);

    //sets port in which the light is connected
    void set_port(const unsigned int& port);
    void set_time_unit(const float& unit);

    private:

    int port_used=0;
    static const String Alphabet[26];
    float time_unit=0.1f;
    void send_dash();
    void send_dot();
    void wait(const unsigned int& time_units);

    public:
    //constructors
    MorseCode(const unsigned int& _port, const float& _time_unit): port_used(_port), time_unit(_time_unit){}
};

//alphabet is in alphabetical order
const String MorseCode::Alphabet[26]={
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

String MorseCode::encrypt(const String& message)
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

void MorseCode::set_port(const unsigned int& port)
{
    port_used=port;
}

void MorseCode::set_time_unit(const float& unit)
{
    time_unit=unit;
}

void MorseCode::wait(const unsigned int& time_units)
{
    //time unit is in seconds, to convert it to the ms we multiply by 1000
    for(unsigned int x=0;x<time_units;x++)
        delay(time_unit*1000);
}

void MorseCode::send_dash()
{
    for(int x=0;x<3;x++)
    {
        digitalWrite(port_used, HIGH);
        wait(3);
        digitalWrite(port_used, LOW);
    }
}

void MorseCode::send_dot()
{
    for(int x=0;x<3;x++)
    {
        digitalWrite(port_used, HIGH);
        wait(1);
        digitalWrite(port_used, LOW);
    }
}

//1 time unit between dots and dashes
//3 time units between letters
//7 time units when the word is completed
void MorseCode::send(const String& encrypted_message)
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
            Serial.println("MorseCode::send: undefined symbol!");
            return;
        }
    }
}

/*
void MorseCodeBasicSetup(const unsigned int& port, const float& time_unit)
{
    pinMode(port, OUTPUT);
    digitalWrite(port, HIGH);

    Serial.begin(9600);
    Serial.setTimeout(120*1000);
}
*/