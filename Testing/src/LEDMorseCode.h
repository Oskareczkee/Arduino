#pragma once
#include <Arduino.h>
//#include <string.h>


//optimized version for LEDs
class MorseCode
{
    public:
    //encrypts to morse code format (dots and dashes)
    static String encrypt(const String& message);

    private:
    static const String Alphabet[26];
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
