#include "../include/fsp.hpp"


FSP::FSP(Nodes _plan) : plan(_plan)
{
  t = 0;
}

FSP::~FSP() {}

void FSP::init(Node* v, Node* g)
{
  Agent::init(v, g);
}

void FSP::actContracted()
{
  Node* u = nextNode();
  if (u == tail) {
    ++t;
    return;
  }
  // check others progress
  if (isStable()) return;

  head = u;
  mode = REQUESTING;
}

void FSP::actExtended()
{
  ++t;  // progress the internal clock
  Agent::actExtended();
}

bool FSP::isStable()
{
  if (mode == CONTRACTED) {
    // reach the goal
    if (tail == goal && t >= plan.size() - 1) return true;
    int _t = t;

    // check whether someone delays
    bool flg = std::all_of(A.begin(), A.end(),
                           [_t] (Agent* a)
                           { FSP* b = reinterpret_cast<FSP*>(a);
                             return _t <= b->getT(); });
    return !flg;
  }

  return (mode == EXTENDED)
    || (mode == REQUESTING && Agent::occupied(head));
}

// go to next node
Node* FSP::nextNode()
{
  if (t + 1 >= plan.size()) return *(plan.end() - 1);  // goal node
  return plan[t+1];
}
