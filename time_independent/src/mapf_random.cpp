#include "../include/mapf_random.hpp"
#include <regex>
#include <fstream>
#include "../include/stn.hpp"

MAPF_RANDOM::MAPF_RANDOM(Graph* _G, Agents _A, std::mt19937* _MT, int _max_activation)
  : Problem(_G, _A, _MT, _max_activation)
{
  setStartsGoalsRandomly();
}

MAPF_RANDOM::MAPF_RANDOM(std::string scen, Graph* _G, Agents _A, std::mt19937* _MT, int _max_activation)
  : Problem(_G, _A, _MT, _max_activation)
{
  readScenario(scen);
  // set starts
  for (int i = 0; i < A.size(); ++i) A[i]->init(starts[i], goals[i]);
}

MAPF_RANDOM::~MAPF_RANDOM() {};

bool MAPF_RANDOM::checkSolved()
{
  for (auto a : A) {
    if (a->getMode() != Agent::CONTRACTED || a->getTail() != a->getGoal()) return false;
  }
  return true;
}

void MAPF_RANDOM::setStartsGoalsRandomly()
{
  starts.clear();
  goals.clear();
  if (Agent::isInitialized()) {
    for (auto s : Agent::CONFIGURATIONS[0]) starts.push_back(s->tail);
  } else {
    starts = getSuffledNodes(A.size());
  }
  // find no duplicated sets
  bool no_duplication = false;
  while (!no_duplication) {
    no_duplication = true;
    goals = getSuffledNodes(A.size());
    for (int i = 0; i < A.size(); ++i) {
      if (starts[i] == goals[i]) {
        no_duplication = false;
        break;
      }
    }
  }

  // set goals
  if (Agent::isInitialized()) {
    for (int i = 0; i < A.size(); ++i) A[i]->setGoal(goals[i]);
  } else {
    for (int i = 0; i < A.size(); ++i) A[i]->init(starts[i], goals[i]);
  }
}

void MAPF_RANDOM::readScenario(std::string filename)
{
  starts.clear();
  goals.clear();

  // read instance file
  std::ifstream file(filename);
  if (!file) halt("file " + filename + " is not found.");

  int num_agents = A.size();

  std::string line;
  std::smatch results;
  std::regex r_sg = std::regex(R"((\d+),(\d+),(\d+),(\d+))");
  while (getline(file, line)) {
    // read initial/goal nodes
    if (std::regex_match(line, results, r_sg)
        && starts.size() < num_agents) {
      int x_s = std::stoi(results[1].str());
      int y_s = std::stoi(results[2].str());
      int x_g = std::stoi(results[3].str());
      int y_g = std::stoi(results[4].str());
      if (!G->existNode(x_s, y_s)) {
        halt("start node ("
             + std::to_string(x_s) + ", " + std::to_string(y_s)
             + ") does not exist, invalid scenario");
      }
      if (!G->existNode(x_g, y_g)) {
        halt("goal node ("
             + std::to_string(x_g) + ", " + std::to_string(y_g)
             + ") does not exist, invalid scenario");
      }

      Node* s = G->getNode(x_s, y_s);
      Node* g = G->getNode(x_g, y_g);
      starts.push_back(s);
      goals.push_back(g);
    }
  }

  if (num_agents > starts.size()) setStartsGoalsRandomly();

  // trimming
  starts.resize(num_agents);
  goals.resize(num_agents);
}

std::string MAPF_RANDOM::strProblem()
{
  std::string str = "problem=MAPF_RANDOM\n";
  str += Problem::strProblem();
  str += "starts=";
  for (auto v : starts) str += std::to_string(v->id) + ",";
  str += "\n";
  str += "goals=";
  for (auto v : goals) str += std::to_string(v->id) + ",";
  str += "\n";

  STN stn = STN(G, A);
  str += "makespan=" + std::to_string(stn.getMakespan()) + "\n";
  str += "soc=" + std::to_string(stn.getSOC()) + "\n";
  str += "mapf_execution=\n";
  str += stn.strMAPFExecution();
  str += "===\n";
  return str;
}
