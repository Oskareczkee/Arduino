#pragma once
#include "Task.h"
#include "Arduino.h"
#include "Utility/vector.h"

/*Task Scheduler class lets you add multiple tasks and run them imitating multi-threaded environment
  Base component of this class is Task class, which you can override for your own purposes - read Task class comments
  Task Scheduler manages Tasks for you, runs them in proper order and has ability to manage them for you
  You can add your own Task class , set it parameters and it will just run - nothing to be worried about easy as it sounds

  This class can be upgraded, for the sake of simplicity it uses just sorted array of tasks sorted by their priority  
  If you do not add too much tasks, it should be fast  
  Another upgrade might be to create this class as global Task Scheduler and improve it, by using more advanced data structures to store Tasks (priority queues)
  But priority queues are expensive to create each iteration, where sorted array is sorted only if needed (check sorting_needed flag)
  A significant upgrade might be to use priority queues if task is added, but to store tasks queue as array, this would prevent creating stack each iteration
*/
class TaskScheduler
{
    private:
        enum class TaskSchedulerFlags{
            RUNNING = 0x01,
            SORTING = 0x02
        };

        vector<Task<void*>*> tasks; //vector of task pointers
        vector<Task<void*>*> tasks_queue; //queue of tasks awaiting to be added
        static unsigned long long actual_id; //ids are global and unique for each task added, id's start from 1, so id==0 means task has not been added yet
        unsigned long next_iterator=0; //iterator for next() function

        /*Task scheduler flags
          1 - is_running [0 - no, 1 - yes] - indicates whether task scheduler is actually in tasks loop
          2 - sorting_needed [0 - no, 1 - yes] - indicates whether tasks queue requires sorting  
        */
        uint8_t flags=0;

        void set_flag(const TaskSchedulerFlags & flag_mask, bool value){
            if((flags & (uint8_t)flag_mask) == value) return;
            else flags ^= (uint8_t)flag_mask;
        }

        bool get_flag(const TaskSchedulerFlags& flag_mask){
            return flags & (uint8_t)flag_mask;
        }

        /*Adds task queued in tasks_queue*/
        void add_from_queue(){

            if(tasks_queue.size()<=0)
                return;
            else
                set_flag(TaskSchedulerFlags::SORTING, true);

            for(int x =0;x<tasks_queue.size();x++)
            {
                tasks.push_back(tasks_queue[x]); //do not run add_task, as it performs sorting every time item is added
                tasks_queue[x]->flip_added_flag();
            }
            tasks_queue.clear();
        }

        /*adds all tasks from tasks queue and sorts them if needed*/
        void unqueue()
        {
            add_from_queue();//add queued tasks

            //if tasks do not require sorting, do not sort them
            if(!get_flag(TaskSchedulerFlags::SORTING))
                return;

            auto arr = tasks.get_array_ptr();
            qsort(arr, tasks.size(), sizeof(Task<void*>*), 
            [](const void* t1, const void* t2) -> int
            {
                Task<void*>* task1 = (Task<void*>*)t1;
                Task<void*>* task2 = (Task<void*>*)t2;

                return task1->GetPriority() > task2->GetPriority();
            });

            set_flag(TaskSchedulerFlags::SORTING, false);
        }

        /*checks all deletion flags and set priority flags*/
        void check_tasks_flags()
        {
            //delete tasks flagged to be deleted
            for(int x =0;x<tasks.size();x++){
                if(tasks[x]->get_delete_flag()){
                    tasks[x]->flip_delete_flag();
                    tasks[x]->flip_added_flag();
                    tasks.erase(x--);
                    continue;
                }

                else if(tasks[x]->get_priority_flag()){
                    set_flag(TaskSchedulerFlags::SORTING, true);
                    tasks[x]->flip_priority_flag();
                }
            }
        }

    public:
        /// @brief Adds task pointer to the task scheduler
        /// @tparam TaskReturnType Return type of task
        /// @param task Task reference / pointer
        template<class TaskReturnType>
        void add_task(Task<TaskReturnType>& task){

            task.set_id(actual_id++);   

            if(get_flag(TaskSchedulerFlags::RUNNING)){ //if is running, add this task to queue instead
                tasks_queue.push_back((Task<void*>*)&task);
                return;
            }

            tasks.push_back((Task<void*>*)&task);
            task.flip_added_flag();

            add_from_queue();//add queued tasks as well

            auto arr = tasks.get_array_ptr();
            qsort(arr, tasks.size(), sizeof(arr[0]), 
            [](const void* t1, const void* t2) -> int
            {
                Task<void*>* task1 = (Task<void*>*)t1;
                Task<void*>* task2 = (Task<void*>*)t2;

                return task1->GetPriority() > task2->GetPriority();
            });
        }

        /// @brief Gets number of tasks in this Task Scheduler
        int get_task_count() const{
            return tasks.size();
        }

        /*Marks task for deletion*/
        void delete_task_by_id(const unsigned long& task_id){
            //find the task in queue
            for(int x =0;x<tasks.size();x++)
                if(tasks[x]->GetID()==task_id){
                    tasks[x]->set_flag(Task<void*>::TaskFlags::DELETE, true);
                    return;
                }
        }

        /// @brief Gets task by its unique ID
        /// @param task_id Task unique ID
        /// @return Task pointer (you have to cast it to proper type) or nullptr if task has not been found
        Task<void*>* get_task_by_id(const unsigned long& task_id){
            //find the task in queue and return it
            for(int x =0;x<tasks.size();x++){
                if(tasks[x]->GetID()==task_id)
                    return tasks[x];
            }

            return nullptr;
        }

        /// @brief Loop of the task scheduler, place it in the loop function of your program
        void loop() {
            set_flag(TaskSchedulerFlags::RUNNING, true);
            for(int x =next_iterator;x<tasks.size();x++){ //use next_iterator value, in case if user will use next() and loop() functions variably

                if(tasks[x]->IsReady()){
                    tasks[x]->Execute();
                    tasks[x]->set_last_execution_time(millis());//TODO: This code should work on arduino platforms, maybe add defines so it will work on other as well?
                    tasks[x]->set_time_delta(millis());
                }
                else
                    tasks[x]->set_time_delta(millis());
            }
            next_iterator=0;//reset next_iterator for next() function
            set_flag(TaskSchedulerFlags::RUNNING, false);
            check_tasks_flags();
            unqueue();
        }

        /// @brief Works like loop function, loops through every task, but every time task is executed task info is returned for your usage
        /// @return TaskInfo<void*> cast it to whatever you expect to be returned
        TaskInfo<void*> next(){
            if(next_iterator >= tasks.size()){
                next_iterator = 0;
                check_tasks_flags();
                unqueue();
            }

            TaskInfo<void*> out;
            if(tasks[next_iterator]->IsReady()){
                out = tasks[next_iterator]->Execute();
                tasks[next_iterator]->set_last_execution_time(millis());//TODO: This code should work on arduino platforms, maybe add defines so it will work on other as well?
                tasks[next_iterator]->set_time_delta(millis());
            }
            else
                tasks[next_iterator]->set_time_delta(millis());

            next_iterator++;
            return out;
        }
};

unsigned long long TaskScheduler::actual_id=1;