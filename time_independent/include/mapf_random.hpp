#pragma once
#include "problem.hpp"

class MAPF_RANDOM : public Problem {
protected:
  Nodes starts;
  Nodes goals;

  bool checkSolved();

  void setStartsGoalsRandomly();
  void readScenario(std::string filename);

public:
  MAPF_RANDOM(Graph* _G,
              Agents _A,
              std::mt19937* _MT,
              int _max_activation);  // random starts and goals
  MAPF_RANDOM(std::string scen,
              Graph* _G,
              Agents _A,
              std::mt19937* _MT,
              int _max_activation);
  ~MAPF_RANDOM();

  std::string strProblem();
};
