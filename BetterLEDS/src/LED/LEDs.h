#pragma once
#include <Arduino.h>
#include <FastLED.h>

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
    // modes are the same as ports
    enum class Mode
    {
        OFF = 2,
        ONE = 3,
        TWO = 4,
        THREE = 5,
        LAST = 6
    };

private:
    enum class LEDFlags
    {
        TURNEDOFF = 0x01,
        GRADIENTREVERSE = 0x02
    };

    unsigned int leds_num;
    CRGB *leds;
    Mode mode = Mode::ONE;
    uint8_t brightness = 58;
    CRGB lightsColor = CRGB{152, 38, 240};

    CRGB GradientFrom{198, 15, 15};
    CRGB GradientTo{53, 1, 89};
    unsigned int GradientIter = 0;

    uint8_t flags = 0x00;

    bool get_flag(const LEDFlags &flag)
    {
        return flags & (uint8_t)flag;
    }

    void set_flag(const LEDFlags &flag, const bool &value)
    {
        if ((flags & (uint8_t)flag) == value)
            return;
        else
            flags ^= (uint8_t)flag;
    }

    void StaticColor() { FastLED.showColor(lightsColor); }

    void Gradient()
    {
        if (GradientIter >= leds_num / 2)
        {
            GradientIter = 0;
            set_flag(LEDFlags::GRADIENTREVERSE, true);
        }

        if (get_flag(LEDFlags::GRADIENTREVERSE))
        {

            float percent = GradientIter / ((float)(leds_num / 2));

            leds[GradientIter] = LinearInterpolation(GradientFrom, GradientTo, percent);
            leds[(leds_num - 1) - GradientIter] = LinearInterpolation(GradientFrom, GradientTo, percent);
            FastLED.show();
        }
        else
        {
            float percent = GradientIter / ((float)(leds_num / 2));

            leds[GradientIter] = LinearInterpolation(GradientTo, GradientFrom, percent);
            leds[(leds_num - 1) - GradientIter] = LinearInterpolation(GradientTo, GradientFrom, percent);
            FastLED.show();
        }

        GradientIter++;
    }

    CRGB LinearInterpolation(const CRGB &color1, const CRGB &color2, const float &percent)
    {
        double R = color1.red + percent * (color2.red - color1.red);
        double G = color1.green + percent * (color2.green - color1.green);
        double B = color1.blue + percent * (color2.blue - color1.blue);

        return CRGB{R, G, B};
    }

    void ChangeModeLamp(const Mode &lastMode, const Mode &newMode)
    {
        digitalWrite((int)lastMode, LOW);
        digitalWrite((int)newMode, HIGH);
    }

public:
    LED(const unsigned int &_port, const unsigned int &_led_num) : leds(new CRGB[_led_num]), leds_num(_led_num) {}
    void setup();

    ~LED() { delete[] leds; }

    int GetPhotoresistorRead() { return analogRead(PHOTORESISTOR_ANALOG); }
    uint8_t GetBrightness() { return brightness; }
    Mode GetMode() { return mode; }
    bool IsButtonClicked() { return digitalRead(BUTTON) == LOW ? true : false; }

    void SetBrightness(uint8_t val)
    {
        brightness = val;
        FastLED.setBrightness(brightness);
    }

    // returns true if was successfull
    bool SetModeInt(unsigned int val)
    {
        if (val > MODENUMBER)
            return false;
        // there is 2 offset casued byt lamp pins
        mode = (Mode)((int)(val + 2));
        return true;
    }

    void TurnOff()
    {
        FastLED.clear();
        FastLED.show();
    }

    void ChangeMode()
    {
        Mode lastMode = mode;

        if (mode == Mode::LAST)
        {
            mode = Mode::OFF;
            TurnOff();
            set_flag(LEDFlags::TURNEDOFF, true);
        }
        else
        {
            mode = (Mode)((int)mode + 1);
            set_flag(LEDFlags::TURNEDOFF, false);
        }

        ChangeModeLamp(lastMode, mode);
    }

    void LightButtonLamp(const int &ms = 1000)
    {
        digitalWrite(BUTTON_LAMP, HIGH);
        delay(ms);
        digitalWrite(BUTTON_LAMP, LOW);
    }

    void loop()
    {
        if (get_flag(LEDFlags::TURNEDOFF))
            return;

        Mode mode = GetMode();
        if (mode == LED::Mode::ONE)
            StaticColor();
        else if (mode == LED::Mode::TWO)
            Gradient();
    }
};

void LED::setup()
{
    Serial.begin(9600);
    Serial.setTimeout(120 * 1000);

    pinMode(PORT, OUTPUT);

    pinMode(BUTTON_LAMP, OUTPUT);
    pinMode(LAMP_OFF, OUTPUT);
    pinMode(LAMP_ONE, OUTPUT);
    pinMode(LAMP_TWO, OUTPUT);
    pinMode(LAMP_THREE, OUTPUT);
    pinMode(LAMP_LAST, OUTPUT);

    digitalWrite(BUTTON_LAMP, LOW);

    // turn off all mode lamps
    for (int x = 2; x <= 6; x++)
        digitalWrite(x, LOW);
    // turn on only acutal mode lamp
    digitalWrite((int)mode, HIGH);

    pinMode(BUTTON, INPUT_PULLUP);

    FastLED.addLeds<WS2812, PORT, GRB>(leds, leds_num);
    FastLED.setBrightness(brightness);
}
