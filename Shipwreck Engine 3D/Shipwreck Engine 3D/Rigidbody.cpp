#include "Rigidbody.h"

#include "StartEngine.h"
using namespace EngineInstance;


Rigidbody::Rigidbody(bool isDynamic_)
{
    physicsManager = &physics;
    isDynamic = isDynamic_;
}

Rigidbody::Rigidbody()
{
    physicsManager = &physics;
    isDynamic = false;
}