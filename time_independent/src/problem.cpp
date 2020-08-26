#include "../include/problem.hpp"

Problem::Problem(Graph* _G, Agents _A, std::mt19937* _MT, int _max_activation)
  : G(_G), A(_A), MT(_MT), max_activation(_max_activation)
{
  if (G->getNodesNum() <= A.size())
    halt("Problem::Problem", "#agent is over #node");

  solved = false;
}

Problem::~Problem() {}


void Problem::run()
{
  if (!Agent::isInitialized()) setStartsRandomly();

  // main loop
  auto t_start = Time::now();

  // pickup randomly then
  for (int i = 0; i < max_activation; ++i) {
    Agent* a = randomChoose(A, MT);
    a->activate();

    if (checkSolved()) {
      solved = true;
      break;
    }
  }

  elapsed = getElapsedTime(t_start);
}

Nodes Problem::getSuffledNodes()
{
  Nodes V = G->getNodes();
  Nodes W(V.size());
  std::copy(V.begin(), V.end(), W.begin());
  std::shuffle(W.begin(), W.end(), *MT);
  return W;
}

Nodes Problem::getSuffledNodes(int n)
{
  if (n >= G->getNodesNum()) {
    halt("Problem::getSuffledNodes", "#node " + std::to_string(n) + " are too big!");
  }
  Nodes V = getSuffledNodes();
  Nodes W;
  for (int i = 0; i < n; ++i) W.push_back(V[i]);
  return W;
}

void Problem::setStartsRandomly()
{
  Nodes starts = getSuffledNodes();
  for (int i = 0; i < A.size(); ++i) A[i]->init(starts[i]);
}

std::string Problem::strProblem()
{
  std::string str = "";
  str += "solved=" + std::to_string(solved) + "\n";
  str += "comp_time=" + std::to_string(elapsed) + "\n";
  return str;
}
