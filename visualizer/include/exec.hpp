#pragma once
#include "graph.hpp"

namespace Agent {
  enum Mode { CONTRACTED, REQUESTING, EXTENDED };
  struct State {
    int id;
    Mode mode;
    Node* head;
    Node* tail;
  };
}

using States = std::vector<Agent::State*>;

struct Exec {
  int num_agents;         // number of agents
  Grid* G;                // grid
  std::string problem_name;
  std::string solver;     // solver name
  bool solved;            // success or not
  Config config_s;        // start configuration
  Config config_g;        // goal configuration
  Configs mapf_exec;      // MAPF execution
  int makespan;           // makespan
  int soc;                // sum of cost
  int comp_time;          // computational time
  std::vector<States> transitions;
  std::vector<int> activated_agents;
  int activated_cnt;
};
