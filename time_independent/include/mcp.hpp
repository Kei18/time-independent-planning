/*
 * Minimum Communication Policies
 */

#pragma once
#include "agent.hpp"


class MCP : public Agent {
 private:
  static std::vector<Path> PLANS;  // MAPF plan

  const Path plan;  // own path
  int t;  // internal clock

  void actContracted();
  void actExtended();

  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  MCP(Nodes _plan);
  ~MCP();

  bool isStable();
  int getT() { return t; }  // used to check temporal dependencies
};
