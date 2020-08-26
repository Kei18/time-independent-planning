/*
 * Causal-PIBT enhanced by MAPF plans
 */

#pragma once
#include "causal_pibt.hpp"


class CausalPIBT_MAPF : public CausalPIBT {
 private:
  Path plan;  // internal plan
  int t;      // internal clock

  void actExtended();
  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  CausalPIBT_MAPF(Nodes _plan);
  ~CausalPIBT_MAPF();
};
