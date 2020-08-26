#include "../include/mapf_dp.hpp"

MAPF_DP::MAPF_DP(Graph* _G,
                 Agents _A,
                 std::mt19937* _MT,
                 float _dp,
                 int _max_activation)
  : MAPF_RANDOM(_G, _A, _MT, _max_activation),
    delayProbMax(_dp)
{
  init();
}

MAPF_DP::MAPF_DP(std::string scen,
                 Graph* _G,
                 Agents _A,
                 std::mt19937* _MT,
                 float _dp,
                 int _max_activation)
  : MAPF_RANDOM(scen, _G, _A, _MT, _max_activation),
    delayProbMax(_dp)
{
  init();
}

MAPF_DP::~MAPF_DP() {}

void MAPF_DP::init()
{
  for (int i = 0; i < A.size(); ++i) {
    delayProbs.push_back(getRandomFloat(0, delayProbMax, MT));
  }
  if (!Agent::isInitialized()) setStartsRandomly();
}


void MAPF_DP::run() {

  // main loop
  auto t_start = Time::now();

  int num_activate = 0;
  bool flg_stop = false;

  while (!flg_stop) {

    Agents unstable;

    // step 1, check transition
    for (auto a : A) {
      if (a->getMode() != Agent::EXTENDED) {
        unstable.push_back(a);
      } else {  // mode == extended

        // skip according to probability
        int i = std::distance(A.begin(), std::find(A.begin(), A.end(), a));
        if (getRandomFloat(0, 1, MT) <= delayProbs[i]) continue;

        a->activate();
        unstable.push_back(a);

        ++num_activate;
        if (num_activate >= max_activation) {
          flg_stop = true;
          break;
        }
      }
    }

    if (flg_stop) break;

    // register all
    States config;
    for (auto a: A) config.push_back(a->getState());
    exec.push_back(config);

    // step 2, check termination
    if (checkSolved()) {
      solved = true;
      break;
    }

    // step 3, interaction
    do {
      while (!unstable.empty()) {
        Agent* a = randomChoose(unstable, MT);

        // activate
        a->activate();
        ++num_activate;
        if (num_activate >= max_activation) {
          unstable.clear();
          flg_stop = true;
          break;
        }

        if (a->isStable()) {
          // remove from unstable
          auto itr = std::find(unstable.begin(), unstable.end(), a);
          unstable.erase(itr);
        }
      }

      if (!flg_stop) {
        for (auto a : A) {
          if (!a->isStable()) unstable.push_back(a);
        }
      }
    } while (!unstable.empty());
  }

  elapsed = getElapsedTime(t_start);
}

int MAPF_DP::getMakespan()
{
  return exec.size() - 1;
}

int MAPF_DP::getSOC()
{
  if (!solved) return 0;
  int soc = 0;
  int makespan = getMakespan();
  for (int i = 0; i < A.size(); ++i) {
    int cost = makespan;
    auto s = exec[cost-1][i];
    while (s->tail == goals[i] && cost > 0) {
      --cost;
      s = exec[cost-1][i];
    }
    soc += cost;
  }
  return soc;
}


std::string MAPF_DP::strProblem()
{
  int makespan = getMakespan();
  int soc = getSOC();

  std::string str = "";
  str += "problem=MAPF_DP\n";
  str += "delay_prob=" + std::to_string(delayProbMax) + "\n";
  str += Problem::strProblem();
  str += "starts=";
  for (auto v : starts) str += std::to_string(v->id) + ",";
  str += "\n";
  str += "goals=";
  for (auto v : goals) str += std::to_string(v->id) + ",";
  str += "\n";
  str += "makespan=" + std::to_string(makespan) + "\n";
  str += "soc=" + std::to_string(soc) + "\n";

  str += "mapf_execution=\n";
  for (int t = 0; t <= makespan; ++t) {
    str += std::to_string(t) + ":";
    for (int i = 0; i < A.size(); ++i) {
      State* s = exec[t][i];
      str += std::to_string(s->tail->id) + ",";
    }
    str += "\n";
  }
  str += "===\n";

  return str;
}
