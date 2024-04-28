#pragma once
#include <WString.h>
#include <Arduino.h>

enum class LogLevel
{
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    NONE= 4
};

class Logger{
        static LogLevel LoggingLevel;
    public:
        static void SetLogLevel(LogLevel level){LoggingLevel = level;}
        static LogLevel GetLogLevel(){return LoggingLevel;}
        static void Info(const String& message)
        {
            if(LoggingLevel >= LogLevel::INFO)
                return;
            Serial.print("[App Info]  ");
            Serial.println(message);
        }
        static void Info(const String& module_name, const String& message)
        {
            if(LoggingLevel >= LogLevel::INFO)
                return;
            Serial.print("["+ module_name +" Info]  ");
            Serial.println(message);
        }

        static void Warning(const String& message)
        {
            if(LoggingLevel >= LogLevel::ERROR)
                return;
            Serial.print("[App Warning]  ");
            Serial.println(message);
        }

        static void Warning(const String& module_name, const String& message)
        {
            if(LoggingLevel >= LogLevel::ERROR)
                return;
            Serial.print("["+ module_name +" Warning]  ");
            Serial.println(message);
        }

        static void Error(const String& message)
        {
            if(LoggingLevel >= LogLevel::NONE)
                return;
            Serial.print("[App Error]  ");
            Serial.println(message);
        }

        static void Error(const String& module_name, const String& message)
        {
            if(LoggingLevel >= LogLevel::NONE)
                return;
            Serial.print("["+ module_name +" Error]  ");
            Serial.println(message);
        }
};

LogLevel Logger::LoggingLevel = LogLevel::INFO;