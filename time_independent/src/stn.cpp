#include "../include/stn.hpp"

STN::STN(Graph* _G, Agents _A) : G(_G), A(_A)
{
  auto t_start = Time::now();

  cnt_id = 0;
  createDiagram();
  findRealization();
  elapsed = getElapsedTime(t_start);
}

STN::~STN() {
  for (auto itr = event_table.begin(); itr != event_table.end(); ++itr) {
    delete itr->second;
  }
}

void STN::createDiagram()
{
  Event* XS = new Event { cnt_id++, -1, nullptr, {}, nullptr };
  event_table[XS->id] = XS;

  Events last_events;  // for each agent
  // initial events
  for (auto s : Agent::ST_DIAGRAM[0]) {
    Event* e = new Event { cnt_id++, 0, s, { XS }, nullptr };
    event_table[e->id] =  e;
    last_events.push_back(e);
  }

  std::unordered_map<int, Event*> last_visits;
  Nodes V = G->getNodes();

  for (auto v : V) {
    if (v != nullptr) last_visits[v->id] = nullptr;
  }

  for (int i = 1; i < Agent::ST_DIAGRAM.size(); ++i) {
    States states = Agent::ST_DIAGRAM[i];

    // 1. internal
    for (int j = 0; j < states.size(); ++j) {
      State* s = states[j];
      Event* e = new Event
        { cnt_id++, i, s, { last_events[s->id] }, nullptr };
      event_table[e->id] = e;
      last_events[s->id] = e;
    }

    // 2. interaction
    for (int j = 0; j < states.size(); ++j) {
      State* s = states[j];
      Event* e = last_events[s->id];
      for (int k = 0; k < states.size(); ++k) {
        if (j == k) continue;
        State* s_prime = states[k];
        Event* e_pre = last_events[s_prime->id]->parents[0];
        e->parents.push_back(e_pre);
      }
    }

    // 3. visit
    for (int j = 0; j < states.size(); ++j) {
      State* s = states[j];
      Event* e = last_events[s->id];
      if (s->mode == Agent::Mode::CONTRACTED &&
          s->pre_mode == Agent::Mode::EXTENDED) {
        Node* last_node = e->parents[0]->s->tail;
        last_visits.at(last_node->id) = e;
      } else if (s->mode == Agent::Mode::EXTENDED) {
        Event* e_pre = last_visits.at(s->head->id);
        if (e_pre != nullptr) e->parents.push_back(e_pre);
      }
    }
  }

  // final event
  XF = new Event
    { cnt_id++, (int)Agent::ST_DIAGRAM.size(), nullptr, {}, nullptr };
  event_table[XF->id] = XF;
  for (auto e : last_events) XF->parents.push_back(e);
}

int STN::getTimestep(Event* e)
{
  return -getDist(e);
}

// get distance to the event XS
int STN::getDist(Event* e)
{
  // check registered or not
  auto itr = event_dists.find(e->id);
  if (itr != event_dists.end()) return itr->second;

  // XS
  if (e->parents.empty()) {
    event_dists.emplace(e->id, 0);
    return 0;
  }

  // find min
  int min_cost = 1;
  int cost;
  Event* earliest;
  for (auto e_pre : e->parents) {
    cost = getDist(e_pre) + edgeWeight(e, e_pre);
    if (cost < min_cost) {
      min_cost = cost;
      earliest = e_pre;
    }
  }

  e->earliest = earliest;
  event_dists.emplace(e->id, min_cost);
  return min_cost;
}

int STN::edgeWeight(Event* child, Event* parent)
{
  // start, final
  if (parent->s == nullptr || child->s == nullptr) return 0;
  if (child->s->id == parent->s->id  // internal
      && child->s->mode == Agent::Mode::EXTENDED  // transition
      && parent->s->mode == Agent::Mode::REQUESTING) {
    return -1;
  }
  return 0;
}

// makespan
int STN::getMakespan()
{
  return getTimestep(XF);
}

// sum-of-cost
int STN::getSOC()
{
  std::vector<int> costs;
  for (auto e: XF->parents) costs.push_back(getTimestep(e));
  return std::accumulate(costs.begin(), costs.end(), 0);
}

// get local execution
Events STN::getLocalExec(Event* last_event)
{
  Events local_exec = {};
  Event* e = last_event;
  while (e->s != nullptr) {
    local_exec.push_back(e);
    e = e->parents[0];
  }
  std::reverse(local_exec.begin(), local_exec.end());
  return local_exec;
}

// extract only constracted events
Events STN::getLocalExecPos(Event* last_event)
{
  Events local_exec = getLocalExec(last_event);
  Events events;

  Node* last_node = nullptr;
  for (auto e : local_exec) {
    if (e->s->mode != Agent::Mode::CONTRACTED) continue;
    if (e->s->tail == nullptr || e->s->tail != last_node) {
      events.push_back(e);
      last_node = e->s->tail;
    }
  }

  return events;
}

// construct MAPF execution
void STN::findRealization()
{
  realization.clear();
  int makespan = getMakespan();

  for (auto last_event : XF->parents) {
    Events events(makespan+1);
    Events local_exec = getLocalExecPos(last_event);

    for (int i = 0; i < local_exec.size(); ++i) {
      Event* e = local_exec[i];
      int t = getTimestep(e);
      events[t] = e;

      // fill gap
      if (i == 0) continue;
      Event* e_pre = local_exec[i-1];
      int t_pre = getTimestep(e_pre);
      for (int j = t_pre + 1; j < t; ++j) events[j] = e_pre;
    }

    // fill last gap
    Event* e_last = local_exec[local_exec.size() - 1];
    int t_last = getTimestep(e_last);
    for (int j = t_last + 1; j <= makespan; ++j) events[j] = e_last;

    realization.push_back(events);
  }
}

// for log
std::string STN::strMAPFExecution()
{
  int makespan = getMakespan();
  std::string str = "";
  for (int t = 0; t <= makespan; ++t) {
    str += std::to_string(t) + ":";
    for (int i = 0; i < A.size(); ++i) {
      auto e = realization[i][t];
      str += std::to_string(e->s->tail->id) + ",";
    }
    str += "\n";
  }
  return str;
}
