#pragma once
#include "agent.hpp"


class FSP : public Agent {
 private:
  const Path plan;
  int t;

  void actContracted();
  void actExtended();

  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  FSP(Nodes _plan);
  ~FSP();

  bool isStable();

  int getT() { return t; }
};
