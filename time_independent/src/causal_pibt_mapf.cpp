#include "../include/causal_pibt_mapf.hpp"

CausalPIBT_MAPF::CausalPIBT_MAPF(Path _plan)
{
  t = 0;
  plan = { _plan[0] };

  for (auto v : _plan) {
    if (v != *(plan.end() - 1)) plan.push_back(v);
  }
}

CausalPIBT_MAPF::~CausalPIBT_MAPF() {}

void CausalPIBT_MAPF::init(Node* v, Node* g)
{
  CausalPIBT::init(v, g);
  pori.path = plan.size();
  Agent::kind_name = "CAUSAL_PIBT_MAPF";
}

void CausalPIBT_MAPF::actExtended()
{
  if ((t+1 < plan.size()) && (plan[t+1] == head)) {
    ++t;
  } else if (inArray(head, plan)) {
    int _t = std::distance(plan.begin(), std::find(plan.begin(), plan.end(), head));
    if (_t > t) t = _t;
  }

  CausalPIBT::actExtended();
}

Node* CausalPIBT_MAPF::nextNode()
{
  if (C.empty()) halt("CausalPIBT::nextNode", "no candidate");

  if (inArray(tail, C)) {
    if (goal == nullptr || tail == goal) return tail;
  }

  if (goal == nullptr) return randomChoose(C, MT);

  Node* v;
  if (t + 1 >= plan.size()) {
    v = *(plan.end() - 1);
    if (inArray(v, C)) {
      return v;
    } else {
      return CausalPIBT::nextNode();
    }
  }

  // usual
  if (plan[t] == tail) {
    v = plan[t+1];
    if (inArray(v, C)) return v;
  }

  // int search_max = (plan.size() < t + 10) ? plan.size() : t + 10;
  int search_max = plan.size();

  // \argmin_{u \in C} { min_{ w \in path[t+1:] } { cost(u, w) } }
  v = *std::min_element(
    C.begin(), C.end(),
    [&] (Node* u1, Node* u2) {
      int d;
      int d_u1 = G->getNodesNum();
      int d_u2 = G->getNodesNum();
      for (int _t = t + 1; _t < search_max; ++_t) {
        d = G->pathDist(u1, plan[_t]);
        if (d < d_u1) d_u1 = d;
        d = G->pathDist(u2, plan[_t]);
        if (d < d_u2) d_u2 = d;
      }

      return d_u1 < d_u2;
    });

  return v;
}
