#include "../include/agent.hpp"
#include <iomanip>

int Agent::cnt_id = 0;
int Agent::cnt_activation = 0;
std::mt19937* Agent::MT;
Graph* Agent::G;
Agents Agent::A;
std::vector<States> Agent::CONFIGURATIONS;
std::vector<States> Agent::ST_DIAGRAM;
std::vector<State*> Agent::GC_state;
Agents Agent::interacted_agents;
bool Agent::is_initialized = false;
bool Agent::verbose = false;


Agent::Agent() : id(cnt_id++)
{
  mode = CONTRACTED;
  pre_mode = CONTRACTED;
  tail = nullptr;
  head = nullptr;
  goal = nullptr;

  A.push_back(this);
}

Agent::~Agent()
{
  for (auto s : GC_state) delete s;
  GC_state.clear();
}

// set tail and goal
void Agent::init(Node* v, Node* g)
{
  Agent::is_initialized = true;
  tail = v;
  goal = g;

  // add configuration
  State* s = getState();
  if (id == 0) {
    ST_DIAGRAM.push_back({ s });
    CONFIGURATIONS.push_back({ s });
  } else {
    ST_DIAGRAM[0].push_back(s);
    CONFIGURATIONS[0].push_back(s);
  }
}

// according to mode, call corresponding activation function
void Agent::activate()
{
  switch (mode) {
  case CONTRACTED:
    actContracted();
    pre_mode = CONTRACTED;
    break;
  case REQUESTING:
    actRequesting();
    pre_mode = REQUESTING;
    break;
  case EXTENDED:
    actExtended();
    pre_mode = EXTENDED;
    break;
  default:
    break;  // ignore
  }

  // update ST_DIAGRAM, CONFIGURATIONS
  States configuration(A.size()), states;
  State* s_activated = getState();
  configuration[id] = s_activated;
  states.push_back(s_activated);
  for (auto a : A) {
    if (a == this) continue;
    State* s = a->getState();
    configuration[a->getId()] = s;
    if (inArray(a, interacted_agents)) states.push_back(s);
  }
  ST_DIAGRAM.push_back(states);
  CONFIGURATIONS.push_back(configuration);
  interacted_agents.clear();

  if (verbose) printState(s_activated);

  ++cnt_activation;
}

/*
 * the following is transition rules
 */
void Agent::actContracted()
{
  // Node* u;
  // head = u;
  // mode = REQUESTING;
}

void Agent::actRequesting()
{
  if (occupied(head)) return;
  mode = EXTENDED;
}

void Agent::actExtended()
{
  tail = head;
  head = nullptr;
  mode = CONTRACTED;
}

bool Agent::occupied(Node* v)
{
  return onAgent(v) != nullptr;
}

// if there is no corresponding agent, just return nullptr
Agent* Agent::onAgent(Node* v)
{
  auto itr = std::find_if(A.begin(), A.end(),
                          [v] (Agent* a) {
                            if (a->getTail() == v) return true;
                            if (a->getMode() == EXTENDED
                                && a->getHead() == v) return true;
                            return false;
                          });
  if (itr == A.end()) return nullptr;
  return *itr;
}

void Agent::interacted(Agent* a)
{
  if (!inArray(a, interacted_agents)) interacted_agents.push_back(a);
}

bool Agent::isStable()
{
  return (mode == EXTENDED)
    || (mode == REQUESTING && Agent::occupied(head))
    || (mode == CONTRACTED && tail == goal);
}

State* Agent::getState()
{
  State* s = new State { id, mode, pre_mode, head, tail, goal };
  GC_state.push_back(s);  // for garbage collection
  return s;
}

std::string Agent::getModeName(Mode mode)
{
  switch (mode) {
  case CONTRACTED:
    return "contracted";
  case REQUESTING:
    return "requesting";
  case EXTENDED:
    return "extended";
  default:
    break;
  }
  return "";
}

void Agent::printState(State* s)
{
  if (s == nullptr) return;
  std::cout << "agent: " << std::right << std::setw(2) << s->id
            << ", mode: " << std::right << std::setw(10) << Agent::getModeName(s->pre_mode)
            << " -> " << std::right << std::setw(10) << Agent::getModeName(s->mode)
            << ", tail: "  << std::right << std::setw(4) << s->tail->id;
  if (s->head != nullptr) {
    std::cout << ", head: " << std::right << std::setw(4) << s->head->id;
  }
  std::cout << "\n";
}

// for log
std::string Agent::strAgent()
{
  std::string str = "";
  str += "execution(id,mode,head,tail,goal)=\n";
  int t = 0;
  for (auto config : CONFIGURATIONS) {
    str += std::to_string(t) + ":";
    str += std::to_string(ST_DIAGRAM[t][0]->id) + ":";
    for (auto state : config) {
      str += "(" + std::to_string(state->id) + ","
        + std::to_string(state->mode) + ",";
      if (state->head != nullptr) {
        str += std::to_string(state->head->id) + ",";
      } else {
        str += "-1,";
      }
      str += std::to_string(state->tail->id) + ","
        + std::to_string(state->goal->id) + "),";
    }
    str += "\n";
    ++t;
  }
  str += "===\n";
  return str;
}
