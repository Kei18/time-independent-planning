#pragma once
#include "graph.hpp"
#include "agent.hpp"
#include "util.hpp"
#include <chrono>

static const int DEFAULT_MAX_ACTIVATION = 1000;


class Problem {
protected:
  Graph* G;
  Agents A;
  std::mt19937* MT;

  bool solved;
  const int max_activation;  // time step limit

  int elapsed;  // time (ms)

  Nodes getSuffledNodes();
  Nodes getSuffledNodes(int n);
  void setStartsRandomly();

  virtual bool checkSolved() { return false; }

public:
  Problem(Graph* _G,
          Agents _A,
          std::mt19937* _MT,
          int _max_activation = DEFAULT_MAX_ACTIVATION);
  virtual ~Problem();

  virtual void run();

  bool isSolved() { return solved; }
  int getElapsed() { return elapsed; }

  virtual std::vector<States> getSyncExec() { return {}; }
  virtual std::string strProblem();
};
