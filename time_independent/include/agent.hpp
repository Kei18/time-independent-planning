#pragma once
#include <random>
#include <iomanip>
#include "graph.hpp"
#include "util.hpp"

class Agent;
using Agents = std::vector<Agent*>;

class Agent {
public:
  // mode
  enum Mode { CONTRACTED, REQUESTING, EXTENDED };

  // used for history
  struct State {
    int id;         // unique id
    Mode mode;      // current mode
    Mode pre_mode;  // mode before activation
    Node* head;     // head
    Node* tail;     // tail
    Node* goal;     // goal
  };
  static std::vector<std::vector<State*>> ST_DIAGRAM;
  static std::vector<std::vector<State*>> CONFIGURATIONS;

private:
  static std::vector<Agent*> interacted_agents;

protected:
  static int cnt_id;             // for unique id
  static int cnt_activation;     // count number of activation
  static std::mt19937* MT;       // seed
  static Graph* G;               // graph
  static Agents A;               // set of all agents
  static bool is_initialized;
  static std::string kind_name;  // algorithm name

  int id;
  Mode mode;
  Mode pre_mode;
  Node* head;
  Node* tail;
  Node* goal;

  virtual void actContracted();
  virtual void actRequesting();
  virtual void actExtended();

  virtual Node* nextNode();
  static void interacted(Agent* a);

public:
  Agent();
  virtual ~Agent();
  virtual void init(Node* v, Node* g = nullptr);

  void activate();

  // getter
  int getId() { return id; }
  Mode getMode() { return mode; }
  Mode getPreMode() { return pre_mode; }
  Node* getHead() { return head; }
  Node* getTail() { return tail; }
  Node* getGoal() { return goal; }
  State* getState();

  // setter
  void setMode(Mode  _mode) { mode = _mode; }
  void setHead(Node* _head) { head = _head; }
  void setTail(Node* _tail) { tail = _tail; }
  void setGoal(Node* _goal) { goal = _goal; }

  // for MAPF-DP
  virtual bool isStable();

  bool operator==(Agent* a) const { return a->getId() == id; };
  bool operator!=(Agent* a) const { return a->getId() != id; };

  static bool occupied(Node* v);
  static Agent* onAgent(Node* v);
  static void setMT(std::mt19937* _MT) { MT = _MT; }
  static void setG(Graph* _G) { G = _G; }
  static std::string getModeName(Mode mode);
  static bool isInitialized() { return is_initialized; }

  static void printState(State* s);
  static std::string strAgent();
};

using State = Agent::State;
using States = std::vector<State*>;
