/*
 * Emulate Multi-Agent Path Finding with Delay Probabilities (MAPF-DP)
 *
 * Ma, H., Kumar, T. K., & Koenig, S. (2016).
 * Multi-agent path finding with delay probabilities.
 * arXiv preprint arXiv:1612.05309.
 */

#pragma once
#include "mapf_random.hpp"


class MAPF_DP : public MAPF_RANDOM {
 private:
  std::vector<float> delayProbs;  // delay probability of each agent
  float delayProbMax;  // maximum delay probability
  std::vector<States> exec;  // mapf execution, main body

  void init();
  int getMakespan();
  int getSOC();

 public:
  MAPF_DP(Graph* _G,
          Agents _A,
          std::mt19937* _MT,
          float _dp,
          int _max_activation);  // random starts and goals
  MAPF_DP(std::string scen,
          Graph* _G,
          Agents _A,
          std::mt19937* _MT,
          float _dp,
          int _max_activation);

  ~MAPF_DP();

  void run();
  std::string strProblem();
};
