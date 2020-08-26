#include "../include/causal_pibt.hpp"

CausalPIBT::CausalPIBT() {}
CausalPIBT::~CausalPIBT() {}

void CausalPIBT::init(Node* v, Node* g)
{
  Agent::init(v, g);

  parent = this;
  children = {};
  pori.has_goal = (int)(!(goal == nullptr || tail == goal));
  pori.cnt_goal_reach = 0;
  pori.id = id;
  pori.path = 0;
  reset();
}

void CausalPIBT::actContracted()
{
  if (C.empty() && parent == this) {
    releaseChildren();
    reset();
  }
  priorityInheritance();

  if (C.empty()) {
    if (parent->getHead() == tail) {
      Nodes S_old = parent->getS();
      Nodes S_new = setUnion(S_old, S);
      Nodes C_old = parent->getC();
      Nodes C_new = setMinus(C_old, S_new);
      parent->setS(S_new);
      parent->setC(C_new);
      parent->setHead(nullptr);
      parent->setMode(CONTRACTED);
      interacted(parent);
    }
    return;
  }

  Node* u = nextNode();
  if (u == tail) {
    releaseChildren();
    reset();
    return;
  }
  removeElement(u, C);
  Nodes tmp = { u, tail };
  S = setUnion(S, tmp);
  head = u;
  mode = REQUESTING;
}

void CausalPIBT::actRequesting()
{
  priorityInheritance();
  Nodes S_parent = parent->getS();
  interacted(parent);

  if (parent != this && inArray(head, S_parent)) {
    head = nullptr;
    mode = CONTRACTED;
    return;
  }

  if (occupied(head)) return;

  CP_Agents R;
  for (int i = 0; i < A.size(); ++i) {
    CausalPIBT* a = reinterpret_cast<CausalPIBT*>(A[i]);
    if (a->getMode() == REQUESTING && a->getHead() == head) {
      R.push_back(a);
    }
  }

  CausalPIBT* winner = chooseWinner(R);
  // losers' action
  for (auto a : R) {
    if (a != winner) {
      a->setHead(nullptr);
      a->setMode(CONTRACTED);
    }
    if (a != this) interacted(a);
  }

  if (winner != this) return;
  if (parent != this) {
    parent->removeChild(this);
    interacted(parent);
  }
  parent = this;
  releaseChildren();
  mode = EXTENDED;
}

void CausalPIBT::actExtended()
{
  tail = head;
  head = nullptr;
  mode = CONTRACTED;

  // update priority
  if (pori.has_goal == 1 && goal != nullptr && tail == goal) {
    pori.cnt_goal_reach += 1;
  }
  pori.has_goal = (int)(!(goal == nullptr || tail == goal));

  reset();
}

CausalPIBT* CausalPIBT::chooseWinner(CP_Agents R)
{
  return *std::max_element(R.begin(), R.end(),
                           [] (CausalPIBT* a, CausalPIBT* b)
                           { return a->getPtmp() < b->getPtmp(); });
}

void CausalPIBT::priorityInheritance()
{
  CP_Agents R;
  for (int i = 0; i < A.size(); ++i) {
    CausalPIBT* a = reinterpret_cast<CausalPIBT*>(A[i]);
    if (a->getMode() == REQUESTING && tail == a->getHead()) {
      R.push_back(a);
    }
  }
  if (R.empty()) return;
  CausalPIBT* a_k = chooseWinner(R);
  Prio ptmp_k = a_k->getPtmp();
  if (ptmp_k <= ptmp) return;

  releaseChildren();
  if (parent != this) {
    parent->removeChild(this);
    interacted(parent);
  }
  parent = a_k;
  a_k->addChild(this);
  ptmp = a_k->getPtmp();
  S = a_k->getS();
  if (mode == REQUESTING) S = setUnion(S, head);  // meaningless when mode is CONTRACTED
  Nodes Neigh = tail->neighbor;
  C = setMinus(Neigh, S);
  interacted(a_k);
}

void CausalPIBT::releaseChildren()
{
  for (auto a : children) {
    a->setParent(a);
    interacted(a);
  }
  children.clear();
}

void CausalPIBT::reset()
{
  S.clear();
  C = tail->neighbor;
  C.push_back(tail);
  ptmp = pori;
}

void CausalPIBT::setParent(CausalPIBT* a)
{
  parent = a;
}

void CausalPIBT::addChild(CausalPIBT* a)
{
  // check validity
  if (inArray(a, children)) {
    halt("CausalPIBT::addChildren", "agent has already added to children");
  }
  children.push_back(a);
}

void CausalPIBT::removeChild(CausalPIBT* a)
{
  // check validity
  if (!inArray(a, children)) {
    halt("CausalPIBT::removeChild", "there is no such agent");
  }
  removeElement(a, children);
}

Node* CausalPIBT::nextNode()
{
  if (C.empty()) halt("CausalPIBT::nextNode", "no candidate");

  if (inArray(tail, C)) {
    if (goal == nullptr || tail == goal) return tail;
  }

  if (goal == nullptr) return randomChoose(C, MT);

  // chose the closest node to the goal
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
  if (min_C.size() == 1) return min_C[0];

  // tie break 1
  Nodes unoccupied;
  for (auto v : min_C) {
    if (!occupied(v)) unoccupied.push_back(v);
  }
  if (unoccupied.size() == 1) return unoccupied[0];

  // tie break 2
  if (!unoccupied.empty()) {
    int max_degree = 0;
    Nodes degrees;
    for (auto v : unoccupied) {
      int d = v->neighbor.size();
      if (d > max_degree) {
        degrees.clear();
        degrees.push_back(v);
        max_degree = d;
      } else if (d == max_degree) {
        degrees.push_back(v);
      }
    }

    return randomChoose(degrees, MT);
  }

  // random
  return randomChoose(min_C, MT);
}

// to adapt MAPF-DP
bool CausalPIBT::isStable()
{
  if (mode == EXTENDED) return true;
  if (mode == REQUESTING) {
    Agent* b = Agent::onAgent(head);
    if (b == nullptr) return false;
    if (b->getMode() == EXTENDED) return true;
  }
  if (mode == CONTRACTED) {
    // stuck
    if (C.empty()) return true;

    // priority inheritance
    if (parent != this) return false;
  }

  // check whether priority inheritance will occur
  CP_Agents R;
  for (int i = 0; i < A.size(); ++i) {
    CausalPIBT* a = reinterpret_cast<CausalPIBT*>(A[i]);
    if (a->getMode() == REQUESTING && tail == a->getHead()) {
      R.push_back(a);
    }
  }
  if (R.empty()) return true;
  CausalPIBT* a_k = chooseWinner(R);
  Prio ptmp_k = a_k->getPtmp();
  if (ptmp_k <= ptmp) return true;

  return false;
}
