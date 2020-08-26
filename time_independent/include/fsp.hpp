/*
 * Fully Synchronous Policies
 */

#pragma once
#include "agent.hpp"


class FSP : public Agent {
 private:
  const Path plan;  // mapf plan
  int t;            // internal clock

  void actContracted();
  void actExtended();

  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  FSP(Nodes _plan);
  ~FSP();

  bool isStable();  // for MAPF-DP
  int getT() { return t; }  // used for synchronization
};
