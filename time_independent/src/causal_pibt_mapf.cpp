#include "../include/causal_pibt_mapf.hpp"

CausalPIBT_MAPF::CausalPIBT_MAPF(Path _plan)
{
  t = 0;
  plan = { _plan[0] };

  // arrange path, skip "stay" action
  for (auto v : _plan) {
    if (v != *(plan.end() - 1)) plan.push_back(v);
  }
}

CausalPIBT_MAPF::~CausalPIBT_MAPF() {}

void CausalPIBT_MAPF::init(Node* v, Node* g)
{
  CausalPIBT::init(v, g);
  pori.path = plan.size();  // set priority
}

void CausalPIBT_MAPF::actExtended()
{
  // increment internal clock
  if ((t+1 < plan.size()) && (plan[t+1] == head)) {
    ++t;
  } else if (inArray(head, plan)) {  // align internal clock
    int _t = std::distance(plan.begin(),
                           std::find(plan.begin(), plan.end(), head));
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
    v = *(plan.end() - 1);  // goal node
    if (inArray(v, C)) {
      return v;  // go to the goal
    } else {
      return CausalPIBT::nextNode();  // work as usual
    }
  }

  // follow the original plan
  if (plan[t] == tail) {
    v = plan[t+1];
    if (inArray(v, C)) return v;
  }

  // \argmin_{u \in C} { min_{ w \in path[t+1:] } { cost(u, w) } }
  v = *std::min_element(
    C.begin(), C.end(),
    [&] (Node* u1, Node* u2) {
      int d;
      int d_u1 = G->getNodesSize();
      int d_u2 = G->getNodesSize();
      for (int _t = t + 1; _t < plan.size(); ++_t) {
        d = G->pathDist(u1, plan[_t]);
        if (d < d_u1) d_u1 = d;
        d = G->pathDist(u2, plan[_t]);
        if (d < d_u2) d_u2 = d;
      }

      return d_u1 < d_u2;
    });

  return v;
}
