#include "../include/greedy.hpp"

Greedy::Greedy()
{
}

Greedy::~Greedy() {}

void Greedy::init(Node* v, Node* g)
{
  Agent::init(v, g);
}

void Greedy::actContracted()
{
  Node* u;
  u = nextNode();
  if (u == tail) return;
  head = u;
  mode = REQUESTING;
}

Node* Greedy::nextNode()
{
  if (goal == nullptr || tail == goal) return tail;
  Nodes C = tail->neighbor;
  C.push_back(tail);

  // pickup the nearest node to the goal
  Nodes min_C;
  int dist;
  int min_dist = G->getNodesNum();
  for (auto v : C) {
    dist = G->pathDist(v, goal);
    if (dist < min_dist) min_C.clear();
    if (dist <= min_dist) {
      min_C.push_back(v);
      min_dist = dist;
    }
  }

  return randomChoose(min_C, MT);
}
