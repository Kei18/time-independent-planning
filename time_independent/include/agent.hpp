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

  // execution history
  // space-time diagram, used to construct simple temporal network
  static std::vector<std::vector<State*>> ST_DIAGRAM;
  // store all configurations
  static std::vector<std::vector<State*>> CONFIGURATIONS;

private:
  // interacted agents, reset for each activation
  static Agents interacted_agents;

  // garbage collection
  static std::vector<State*> GC_state;

protected:
  static int cnt_id;             // for unique id
  static int cnt_activation;     // number of activation
  static std::mt19937* MT;       // seed
  static Graph* G;               // graph
  static Agents A;               // set of all agents
  static bool is_initialized;    // whether agents are given initial locations

  static bool verbose;  // print additional info

  // see "struct State""
  int id;
  Mode mode;
  Mode pre_mode;
  Node* head;
  Node* tail;
  Node* goal;

  // activation function
  virtual void actContracted();  // activated when mode = contracted
  virtual void actRequesting();  // activated when mode = requesting
  virtual void actExtended();    // activated when mode = extended

  // register interacted agents
  static void interacted(Agent* a);

  // whether a node v is occupied
  static bool occupied(Node* v);

  // return an agent on a node v
  static Agent* onAgent(Node* v);

public:
  Agent();
  virtual ~Agent();
  virtual void init(Node* v, Node* g = nullptr);

  // call actContracted, actRequesting, or actExtended
  void activate();

  // getter
  int getId() { return id; }
  Mode getMode() { return mode; }
  Mode getPreMode() { return pre_mode; }
  Node* getHead() { return head; }
  Node* getTail() { return tail; }
  Node* getGoal() { return goal; }
  State* getState();
  static bool isInitialized() { return is_initialized; }

  // setter
  void setMode(Mode  _mode) { mode = _mode; }
  void setHead(Node* _head) { head = _head; }
  void setTail(Node* _tail) { tail = _tail; }
  void setGoal(Node* _goal) { goal = _goal; }

  bool operator==(Agent* a) const { return a->getId() == id; };
  bool operator!=(Agent* a) const { return a->getId() != id; };

  // used in MAPF-DP
  virtual bool isStable();

  // setting
  static void setMT(std::mt19937* _MT) { MT = _MT; }
  static void setG(Graph* _G) { G = _G; }
  static void setVerbose(bool flg) { verbose = flg; }

  // print current status
  static void printState(State* s);

  // mode name
  static std::string getModeName(Mode mode);

  // for log
  static std::string strAgent();
};

using State = Agent::State;
using States = std::vector<State*>;
