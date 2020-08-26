#pragma once
#include "causal_pibt.hpp"


class CausalPIBT_MAPF : public CausalPIBT {
 private:
  Nodes plan;
  int t;

  void actExtended();
  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  CausalPIBT_MAPF(Nodes _plan);
  ~CausalPIBT_MAPF();
};
