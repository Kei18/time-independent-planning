#pragma once
#include "agent.hpp"
#include <unordered_map>

struct Event;
using Events = std::vector<Event*>;
using Realization = std::vector<Events>;

struct Event {
  int id;
  int t;
  State* s;
  Events parents;
  Event* earliest;
};

class STN {
private:
  Graph* G;
  Agents A;

  int elapsed;

  int cnt_id;
  Event* XF;  // initial events
  Realization realization;
  std::unordered_map<int, Event*> event_table;
  std::unordered_map<int, int> event_dists;

  void createDiagram();
  int getTimestep(Event* e);
  int getDist(Event* e);  // recursive func
  static int edgeWeight(Event* child, Event* parent);
  static Events getLocalExec(Event* last_event);
  static Events getLocalExecPos(Event* last_event);

  void findRealization();

public:
  STN(Graph* G, Agents _A);
  ~STN();

  int getMakespan();
  std::vector<int> getCosts();
  int getSOC();
  Realization getRealization() { return realization; }
  void printPath();

  std::string strMAPFExecution();
  std::string strSTN();
};
