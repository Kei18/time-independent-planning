#include "ofMain.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "../include/ofApp.hpp"
#include "../include/graph.hpp"
#include "../include/exec.hpp"

void readSetNode(const std::string& s, Config& config, Grid* G);
void readSetStates(const std::string& s, States& states, Grid* G);
void readSetResult(const std::string& result_file, Exec* exec);


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Put your result file as the first arg." << std::endl;
    return 0;
  }

  Exec* exec = new Exec;
  readSetResult(argv[1], exec);

  // simple check
  if (exec->transitions.empty()) {
    warn("the result cannot be visualized.");
    return 0;
  }

  ofSetupOpenGL(100, 100, OF_WINDOW);
  ofRunApp(new ofApp(exec));

  delete exec;
  return 0;
}

void readSetNode(const std::string& s, Config& config, Grid* G)
{
  if (G == nullptr) halt("graph is not read.");
  std::regex r_pos = std::regex(R"((\d+),)");
  std::smatch m;
  auto iter = s.cbegin();
  while (std::regex_search(iter, s.cend(), m, r_pos)) {
    iter = m[0].second;
    int i = std::stoi(m[1].str());
    if (!G->existNode(i))
      halt("node " + std::to_string(i) + " does not exist");
    config.push_back(G->getNode(i));
  }
}

void readSetStates(const std::string& s, States& states, Grid* G)
{
  if (G == nullptr) halt("graph is not read.");
  std::regex r_state = std::regex(R"(\((\d+),(\d+),([-,0-9]+),(\d+),(\d+)\),)");
  std::smatch m;
  auto iter = s.cbegin();
  while (std::regex_search(iter, s.cend(), m, r_state)) {
    iter = m[0].second;

    int i;
    Node* head = nullptr;
    i = std::stoi(m[3].str());
    if (i != -1) {
      if (!G->existNode(i))
        halt("node " + std::to_string(i) + " does not exist");
      head = G->getNode(i);
    }
    Node* tail;
    i = std::stoi(m[4].str());
    if (!G->existNode(i))
      halt("node " + std::to_string(i) + " does not exist");
    tail = G->getNode(i);

    Agent::State* state = new Agent::State
      {
       std::stoi(m[1].str()),
       static_cast<Agent::Mode>(std::stoi(m[2].str())),
       head,
       tail
      };
    states.push_back(state);
  }
}

void readSetResult(const std::string& result_file, Exec* exec)
{
  std::ifstream file(result_file);
  if (!file) halt("file " + result_file + " is not found.");

  std::regex r_map       = std::regex(R"(map_file=(.+))");
  std::regex r_agents    = std::regex(R"(agents=(.+))");
  std::regex r_solver    = std::regex(R"(solver=(.+))");
  std::regex r_solved    = std::regex(R"(solved=(\d))");
  std::regex r_problem   = std::regex(R"(problem=(.+))");
  std::regex r_starts    = std::regex(R"(starts=(.+))");
  std::regex r_goals     = std::regex(R"(goals=(.+))");
  std::regex r_makespan  = std::regex(R"(makespan=(\d+))");
  std::regex r_soc       = std::regex(R"(soc=(\d+))");
  std::regex r_comp_time = std::regex(R"(comp_time=(\d+))");
  std::regex r_mapf_exec = std::regex(R"(mapf_execution=)");
  std::regex r_mapf_row  = std::regex(R"(\d+:(.+))");
  std::regex r_exec      = std::regex(R"(execution.+)");
  std::regex r_exec_row  = std::regex(R"((\d+):(\d+):(.+))");
  std::regex r_cnt_activation = std::regex(R"(cnt_activation=(\d+))");

  std::string line;
  std::smatch results;
  while (getline(file, line)) {
    // read map
    if (std::regex_match(line, results, r_map)) {
      exec->G = new Grid(results[1].str());
      continue;
    }
    // set agent num
    if (std::regex_match(line, results, r_agents)) {
      exec->num_agents = std::stoi(results[1].str());
      continue;
    }
    // solver
    if (std::regex_match(line, results, r_solver)) {
      exec->solver = results[1].str();
      continue;
    }
    // problem
    if (std::regex_match(line, results, r_problem)) {
      exec->problem_name = results[1].str();
      continue;
    }
    // solved?
    if (std::regex_match(line, results, r_solved)) {
      exec->solved = (bool)std::stoi(results[1].str());
      continue;
    }
    // makespan
    if (std::regex_match(line, results, r_makespan)) {
      exec->makespan = std::stoi(results[1].str());
      continue;
    }
    // soc
    if (std::regex_match(line, results, r_soc)) {
      exec->soc = std::stoi(results[1].str());
      continue;
    }
    // comp_time
    if (std::regex_match(line, results, r_comp_time)) {
      exec->comp_time = std::stoi(results[1].str());
      continue;
    }
    // starts
    if (std::regex_match(line, results, r_starts)) {
      readSetNode(results[1].str(), exec->config_s, exec->G);
      continue;
    }
    // goals
    if (std::regex_match(line, results, r_goals)) {
      readSetNode(results[1].str(), exec->config_g, exec->G);
      continue;
    }
    // mapf execution
    if (std::regex_match(line, results, r_mapf_exec)) {
      while (getline(file, line)) {
        if (std::regex_match(line, results, r_mapf_row)) {
          Config c;
          readSetNode(results[1].str(), c, exec->G);
          exec->mapf_exec.push_back(c);
        } else {
          break;
        }
      }
      continue;
    }
    // activation count
    if (std::regex_match(line, results, r_cnt_activation)) {
      exec->activated_cnt = std::stoi(results[1].str());
      continue;
    }
    // execution
    if (std::regex_match(line, results, r_exec)) {
      while (getline(file, line)) {
        if (std::regex_match(line, results, r_exec_row)) {
          States states;
          exec->activated_agents.push_back(std::stoi(results[2].str()));
          readSetStates(results[3].str(), states, exec->G);
          exec->transitions.push_back(states);
        } else {
          break;
        }
      }
      continue;
    }
  }
}
