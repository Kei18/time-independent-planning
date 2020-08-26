#pragma once
#include "mapf_random.hpp"


class MAPF_DP : public MAPF_RANDOM {
 private:
  std::vector<float> delayProbs;
  float delayProbMax;
  std::vector<States> exec;

  void init();
  int getMakespan();
  int getSOC();

 public:
  MAPF_DP(Graph* _G,
         Agents _A,
         std::mt19937* _MT,
         float _dp,
         int _max_activation);
  MAPF_DP(std::string scen,
         Graph* _G,
         Agents _A,
         std::mt19937* _MT,
         float _dp,
         int _max_activation);

  ~MAPF_DP();

  void run();
  std::vector<States> getSyncExec() { return exec; }

  std::string strProblem();
};
