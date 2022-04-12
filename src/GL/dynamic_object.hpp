#pragma once

#include <unordered_set>

namespace GL {

class DynamicObject
{
public:
    virtual ~DynamicObject() {}

    // TASK-0 C-4: remove aircraft.
    // We need some way to know if a DynamicObject needs to be removed.
    // Here, we decided to have a value returned from move.
    // We also renamed it 'update', because update -> bool is quite classic to indicate whether something
    // still needs to be updated, whereas move -> bool would probably make us think that the return value
    // indicates whether the plane is moving or not.
    virtual bool move() = 0;
};

inline std::unordered_set<DynamicObject*> move_queue;

} // namespace GL
