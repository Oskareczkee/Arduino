#include "TaskScheduler/Task.h"
#include <Arduino.h>

class TestTask : public Task<int>{
    private:
        String message;
    public:
    TaskInfo<int> Execute() override{
        Serial.println(message);
        return TaskInfo<int>(TaskState::Success);
    }

    void setMessage(const String& mess){
        message=mess;
    }
};