#include "../include/problem.hpp"

Problem::Problem(Graph* _G,
                 Agents _A,
                 std::mt19937* _MT,
                 int _max_activation)
  : G(_G), A(_A), MT(_MT), max_activation(_max_activation)
{
  if (G->getV().size() <= A.size())
    halt("Problem::Problem", "#agent is over #node");

  solved = false;
}

Problem::~Problem() {}


void Problem::run()
{
  if (!Agent::isInitialized()) setStartsRandomly();
  auto t_start = Time::now();

  // pickup randomly then activate
  for (int i = 0; i < max_activation; ++i) {
    Agent* a = randomChoose(A, MT);
    a->activate();
    if (checkSolved()) {  // check termination
      solved = true;
      break;
    }
  }

  elapsed = getElapsedTime(t_start);
}

Nodes Problem::getShuffledNodes()
{
  Nodes V = G->getV();
  Nodes W(V.size());
  std::copy(V.begin(), V.end(), W.begin());
  std::shuffle(W.begin(), W.end(), *MT);
  return W;
}

Nodes Problem::getShuffledNodes(int n)
{
  Nodes V = getShuffledNodes();
  V.resize(n);
  return V;
}

void Problem::setStartsRandomly()
{
  Nodes starts = getShuffledNodes();
  for (int i = 0; i < A.size(); ++i) A[i]->init(starts[i]);
}

std::string Problem::strProblem()
{
  std::string str = "";
  str += "solved=" + std::to_string(solved) + "\n";
  str += "comp_time=" + std::to_string(elapsed) + "\n";
  return str;
}
