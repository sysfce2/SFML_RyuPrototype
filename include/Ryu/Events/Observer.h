#pragma once

#include <string>
#include <Ryu/Events/EventEnums.h>

class SceneNode;

class Observer
{

public:
  explicit Observer(std::string _name){mName=_name;}
  std::string getObserverName(){return mName;}
  virtual ~Observer() {}
  virtual void onNotify(const SceneNode& entity, Event event) = 0;

protected:
  std::string mName;
};
