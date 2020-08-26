/*
 * Simple Temporal Network
 *
 * used to convert time-independent planning to MAPF execution
 *
 * ref
 * Hönig, W., Kumar, T. S., Cohen, L., Ma, H., Xu, H., Ayanian, N., & Koenig, S. (2016, June).
 * Multi-Agent Path Finding with Kinematic Constraints.
 * In ICAPS (Vol. 16, pp. 477-485).
 */

#pragma once
#include "agent.hpp"
#include <unordered_map>

struct Event;
using Events = std::vector<Event*>;
using Realization = std::vector<Events>;

struct Event {
  int id;           // event id
  int t;            // timestep
  State* s;         // agent's state
  Events parents;   // parents, i.e., temporal dependencies
  Event* earliest;  // used when finding minimum path
};

class STN {
private:
  Graph* G;  // graph
  Agents A;  // agents

  int elapsed;  // time (ms)

  int cnt_id;  // used for event id
  Event* XF;   // initial events
  Realization realization;  // MAPF execution
  // register all events
  std::unordered_map<int, Event*> event_table;
  // distance to the initial event
  std::unordered_map<int, int> event_dists;

  void createDiagram();       // create space-time diagram
  int getTimestep(Event* e);  // get earliest timestep
  int getDist(Event* e);      // recursive func, used in getTimestep
  void findRealization();     // get MAPF execution
  static int edgeWeight(Event* child, Event* parent);  // used in getDist
  static Events getLocalExec(Event* last_event);
  static Events getLocalExecPos(Event* last_event);

public:
  STN(Graph* G, Agents _A);
  ~STN();

  int getMakespan();
  int getSOC();
  std::string strMAPFExecution();
};
