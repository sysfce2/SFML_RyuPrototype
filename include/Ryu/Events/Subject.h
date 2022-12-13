// © 85K Games 2022

#pragma once
#include <Ryu/Events/EventEnums.h>


/* This class is part of the observer-pattern used to notice certain classes, when something of relevance happen.
   A class which sends information is called subject and it notifies other classes or objects (observer).
   A class which acts as an subject needs to inherit from the class 'Subject'. 
*/

class Observer;
class SceneNode;

static constexpr int MAX_OBSERVERS = 10;

class Subject
{

public:
    Subject();
    ~Subject();
    void addObserver(Observer* observer);

    void removeObserver(Observer* observer);

protected:
    void notify(const SceneNode& entity, Event event);

private:
    // approach without dynamic memallocation
    Observer* observers_[MAX_OBSERVERS];
    int numObservers_;
};
