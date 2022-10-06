#pragma once
#include <Ryu/Events/EventEnums.h>

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
    // aaproach without dynamic memallocation
    Observer* observers_[MAX_OBSERVERS];
    int numObservers_;
};
