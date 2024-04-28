#pragma once
#include "TaskInfo.h"
#include "Task.h"

/// @brief Class representing Task which execution is partitioned into smaller parts and executed in some period of time
/// @tparam T Return value type
template<class T>
class PartitionedTask : public Task<T>
{
    
};