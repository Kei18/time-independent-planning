#include "../include/mcp.hpp"

std::vector<Path> MCP::PLANS;


MCP::MCP(Nodes _plan) : plan(_plan)
{
  t = 0;
  PLANS.push_back(plan);
}

MCP::~MCP() {
}

void MCP::init(Node* v, Node* g) {
  Agent::init(v, g);
  Agent::kind_name = "MCP";
}

void MCP::actContracted() {
  Node* u = nextNode();
  if (u == tail) {
    ++t;
    return;
  }
  if (isStable()) return;

  head = u;
  mode = REQUESTING;
}

void MCP::actExtended() {
  ++t;
  Agent::actExtended();
}

bool MCP::isStable() {
  if (mode != CONTRACTED) {
    return (mode == EXTENDED) || (mode == REQUESTING && Agent::occupied(head));
  }

  // contracted
  if (tail == goal && t >= plan.size() - 1) return true;
  Node* u = nextNode();

  // check dependencies
  for (int i = 0; i < A.size(); ++i) {
    for (int j = 0; j <= t; ++j) {
      if (j >= PLANS[i].size()) break;
      if (u != PLANS[i][j]) continue;
      MCP* a = reinterpret_cast<MCP*>(A[i]);
      if (a->getT() < t) return true;
    }
  }

  return false;
}

Node* MCP::nextNode() {
  if (t > plan.size()) return *(plan.end() - 1);
  return plan[t+1];
}
