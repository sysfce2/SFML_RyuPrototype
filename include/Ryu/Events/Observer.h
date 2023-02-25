// © 85K Games 2022

#pragma once

/* This class is part of the observer-pattern used to notice certain classes, when something of relevance happen.
   A class which receives information is called observer and it gets events from other classes or objects (subjects).
   A class which acts as an observer needs to inherit from the class 'Observer'. 
*/

#include <string>
#include <Ryu/Events/EventEnums.h>

class SceneNode;

class Observer
{

public:
  explicit Observer(std::string _name){mName=_name;}
  std::string getObserverName(){return mName;}
  virtual ~Observer() {}
  virtual void onNotify(const SceneNode& entity, EEvent event) = 0;

protected:
  std::string mName;
};
