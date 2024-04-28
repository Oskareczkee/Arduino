#pragma once

enum class TaskState
{
    None, Error, Success, NotExecuted
};

template<class T>
struct TaskInfo
{
  TaskState state = TaskState::NotExecuted;
  T return_val;
  bool IsError(){return state!=TaskState::Success;}

  TaskInfo(){}
  TaskInfo(const TaskState& state){this->state = state;}
  TaskInfo(const TaskState& state, const T& return_val){this->state = state; this->return_val = return_val;}
};