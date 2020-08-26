#pragma once
#include "agent.hpp"


class MCP : public Agent {

 private:
  static std::vector<Path> PLANS;

  const Path plan;
  int t;

  void actContracted();
  void actExtended();

  void init(Node* v, Node* g = nullptr);
  Node* nextNode();

 public:
  MCP(Nodes _plan);
  ~MCP();

  bool isStable();
  int getT() { return t; }
};
