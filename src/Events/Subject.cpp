#include <iostream>

#include <Ryu/Events/Subject.h>
#include <Ryu/Events/Observer.h>
#include <Ryu/Events/EventEnums.h>

#include <fmt/core.h>

using EEvent = Ryu::EEvent;

Subject::Subject()
:   numObservers_(0)
{
}

Subject::~Subject()
{
}

void
Subject::addObserver(Observer* observer)
{
    // Add to array...
    observers_[numObservers_] = observer;
    ++numObservers_;
}

void
Subject::removeObserver(Observer* observer)
{
    // TODO: refine / order observer continuesly of one is deleted ...
    if(numObservers_>0)
    {
        // Remove from array...
        for(int i=0;i<numObservers_;i++)
        {
            if (observers_[i] == observer)
            {
                observers_[i] = nullptr;
            }
        }
        numObservers_--;
    }
    
}

void
Subject::notify(const SceneNode& entity, EEvent event)
{
    if(numObservers_ > 0)
    {
        for (int i=0;i<numObservers_;i++)
        {
            //fmt::print("Notify observer {}\n",observers_[i]->getObserverName());
            observers_[i]->onNotify(entity, event);
        }
    }
    
}
