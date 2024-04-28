#pragma once
#include "TaskInfo.h"

/// @brief Class representing Task, that can be executed in some period of time
/// Base class for all tasks, which should be derived.
/// Overload virtual method Execute for your own use
/// @tparam T Return value type
template<class T>
class Task
{
    private:
        enum class TaskFlags{
            PRIORITY = 0x01,
            DELETE = 0x02,
            ADDED = 0x04
        };

        friend class TaskScheduler;

        unsigned long long _timeout_ms=0;
        unsigned long long _time_delta=0;
        long long _time_last_execution=0;
        unsigned long long  _id=0;
        unsigned long long  _priority =0;

        //task metadata, may be usefull for someone to use it
        String _name="";
        String _return_type="";

        /*Bits:
          -1 - priority changed [0 - not changed, 1 - changed]
          -2 - delete flag [0 - do not delete me, 1 - delete me]
          -3 - has been added to task scheduler [0 - no, 1 - yes]
        */
        uint8_t flags =0x00;
                               

        void set_id(const unsigned long& id){_id = id;}
        void set_priority(const unsigned long& priority){_priority=priority;}
        void set_time_delta(const unsigned long long& actual_time_ms){_time_delta = (actual_time_ms - _time_last_execution);}
        void set_last_execution_time(const unsigned long long& actual_time_ms){_time_last_execution = actual_time_ms;}

        //get state functions

        bool get_priority_flag()    {return flags & (uint8_t)TaskFlags::PRIORITY;}
        bool get_delete_flag()      {return flags & (uint8_t)TaskFlags::DELETE;}
        bool get_added_flag()       {return flags & (uint8_t)TaskFlags::ADDED;}

        //flip state functions

        void flip_priority_flag() {flags ^= (uint8_t)TaskFlags::PRIORITY;}
        void flip_delete_flag()   {flags ^= (uint8_t)TaskFlags::DELETE;}
        void flip_added_flag()    {flags ^= (uint8_t)TaskFlags::ADDED;}

        void set_flag(const TaskFlags& flag_mask, const bool& value){
            if((flags & (uint8_t)flag_mask) == value) return;
            else flags ^= (uint8_t)flag_mask;
        }

    public:
        /// @brief Function to be executed, does nothing unless you write your own task class
        /// To write your own task class, just derive this class with public modifier
        /// Add whatever you need and override this method, this is the code your task will be executing
        /// @return TaskInfo class, info about task execution, return value, whether it succeded or not, just check it
        virtual TaskInfo<T> Execute(){};
        /// @brief Returns whether this task is ready to execute - timeout time passed
        bool IsReady(){return _time_delta>= _timeout_ms;}

        /// @brief Gets this task unique ID
        unsigned long GetID()const {return _id;}
        /// @brief Returns Task priority, the bigger the priority, the more important the task is
        unsigned long GetPriority()const {return _priority;}
        /// @brief Returns task timeout
        unsigned long int GetTimeout(){return _timeout_ms;}
        /// @brief Returns last time when task was executed, the time is platform dependent
        /// On Arduino this will be time from the start of the module
        /// @return Time from module start in milliseconds
        long long GetLastTimeExecution(){return _time_last_execution;}
        /// @brief Returns task name, returns empty string if name was not set
        String GetName(){return _name;}
        /// @brief Returns task return type, returns empty string if return type was not set
        String GetReturnType(){return _return_type;}


        /// @brief Sets timeout after which task will be executed, set to 0, to execute it without timeout delay
        /// @param timeout_ms Timeout in milliseconds
        void SetTimeout(const unsigned long int& timeout_ms){_timeout_ms = timeout_ms;}
        /// @brief Sets priority of this task
        /// @param priority The bigger the priority, the more important task is
        void SetPriority(const unsigned long& priority){_priority=priority; set_flag(TaskFlags::PRIORITY, true);}
        /// @brief Sets Tasks name, use it for your own purpose
        void SetName(const String& name){_name=name;}
        /// @brief Sets Tasks return type string, use it for your own purpose
        void SetReturnTypeString(const String& return_type){_return_type = return_type;}
        /// @brief Sets Tasks name and return type metadata
        /// @param name Name of task to be set
        /// @param return_type Return type of task to be set
        void SetMetadata(const String& name, const String& return_type){_name = name; _return_type = return_type;}

        /// @brief Sets Task to be deleted in Task Scheduler
        void Delete(){set_flag(TaskFlags::DELETE,true);}

        //task flags getters

        /// @brief Returns whether this task has been added to any task scheduler
        bool IsAdded(){return get_added_flag();}
        /// @brief Returns whether this task is marked to deletion
        bool IsDeleted(){return get_delete_flag();}
        /// @brief Returns whether priority of this task has been changed in task scheduler
        bool IsPriorityChanged(){return get_priority_flag();}
};