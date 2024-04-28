#include "TaskScheduler/Task.h"
#include "LEDs.h"

class LEDTask : public Task<bool>
{
    private:
        LED* leds_ref;
    public:
        LEDTask(LED& leds){leds_ref = &leds;}

        TaskInfo<bool> Execute() override{
            leds_ref->loop();
            return TaskInfo<bool>(TaskState::Success, true);
        }
};