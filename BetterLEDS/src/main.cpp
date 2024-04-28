#include <Arduino.h>
#include "TaskScheduler/TaskScheduler.h"
#include "LED/LEDs.h"
#include "LED/ButtonListenerTask.h"
#include "LED/LEDTask.h"
#include "TestTask.h"

LED leds(7,300);

TaskScheduler ts;
ButtonListener bl(leds);
LEDTask lt(leds);

void setup() {
    leds.setup();
    bl.SetPriority(10);

    bl.SetTimeout(200);
    lt.SetTimeout(5);

    ts.add_task(bl);
    ts.add_task(lt);
}

void loop() {
    ts.loop();
}