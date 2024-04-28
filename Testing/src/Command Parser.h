#pragma once
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector.cpp>
#include <string.h>

using namespace std;


//we use String instead of string, because arduino uses String not string
vector<string> split(const String& _str, const String& _delim)
{
    vector<string> tokens;

    string str(_str.c_str());
    string delim(_delim.c_str());

    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());

    return tokens;
}

struct Command
{
    string command;
    vector<string> args;
};

class CommandParser
{
    public:
    static Command parseCommand(const String& command);
    virtual void Evaluate(const Command& command)=0;
};

Command CommandParser::parseCommand(const String& command)
{
    auto output = split(command, " ");

    Command outputCommand;

    outputCommand.command=output[0];
    for(int x =1;x<output.size();x++)
        outputCommand.args.push_back(output[x]);

    return outputCommand;
}

/*
        class LEDCommandParser : public CommandParser
        {
            private:
            LED& leds;

            public:

            LEDCommandParser(LED& led): leds(led){}
            virtual void Evaluate(const Command& command) override
            {
                if(command.command=="set_brightness")
                {     
                    if(command.args.size()==0 || command.args.size()>1)
                    {
                        Serial.println("Function: set_brightness - too many arguments");
                        return;
                    }


                    int val = String(command.args[0].c_str()).toInt();
                    leds.setBrightness(val);

                    return;
                }
                else
                {
                    Serial.println("Command not found !");
                }
            }  
        };
*/