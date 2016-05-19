#include "ccinfra/memory/SharedObject.h"

SharedObject::SharedObject() : count(0)
{
}

SharedObject::~SharedObject()
{
}

void SharedObject::addRef()
{
    count++;
}

unsigned int SharedObject::getRefCount() const
{
    return count;
}

bool SharedObject::onlyThisRef() const
{
    return count == 1;
}

void SharedObject::subRef()
{
    if(count > 0)
    {
        count--;
    }

    if(count == 0)
    {
        needDestroy() ?  destroy() : delete this;
    }
}
