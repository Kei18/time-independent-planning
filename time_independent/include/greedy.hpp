#pragma once
#include "agent.hpp"


class Greedy : public Agent {
protected:
  virtual void init(Node* v, Node* g = nullptr);
  virtual Node* nextNode();

public:
  Greedy();
  ~Greedy();
};
