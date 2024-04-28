#include "TaskScheduler/Task.h"
#include "LEDs.h"

class ButtonListener : public Task<bool>{
    private:
        LED* leds_ref;
    public:
        ButtonListener(LED& leds){leds_ref = &leds;}

        TaskInfo<bool> Execute() override{
            if(leds_ref->IsButtonClicked()){
                leds_ref->LightButtonLamp(500);
                leds_ref->ChangeMode();

                return TaskInfo<bool>(TaskState::Success, true);
            }
           return TaskInfo<bool>(TaskState::Success, false);
        }
};