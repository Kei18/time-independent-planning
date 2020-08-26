#pragma once
#include "agent.hpp"

class CausalPIBT;
using CP_Agents = std::vector<CausalPIBT*>;

struct Prio {  // priority
  int has_goal;        // has goal or not
  int cnt_goal_reach;  // step
  int id;              // unique
  int path;            // path, used with mapf plan

  bool operator==(const Prio& other) const {
    return (has_goal == other.has_goal &&
            cnt_goal_reach == other.cnt_goal_reach &&
            path == other.path &&
            id == other.id);
  }

  bool operator!=(const Prio& other) const {
    return !(*this == other);
  }

  bool operator<(const Prio& other) const {
    if (has_goal != other.has_goal) {
      return has_goal < other.has_goal;
    } else if (cnt_goal_reach != other.cnt_goal_reach) {
      return -cnt_goal_reach < -other.cnt_goal_reach;
    } else if (path != other.path) {
      return -path < -other.path;
    } else {
      return id < other.id;
    }
  }

  bool operator<=(const Prio& other) const {
    return (*this == other || *this < other);
  }

  bool operator>(const Prio& other) const {
    return !(*this <= other);
  }

  bool operator>=(const Prio& other) const {
    return (*this == other || *this > other);
  }

  void println() {
    std::cout << std::right << std::setw(3) << id << ", "
              << "has_goal:" << std::right << std::setw(6) << has_goal  << ", "
              << "cnt:" << std::right << std::setw(6) << cnt_goal_reach << ", "
              << "id:" << std::right << std::setw(6) << id << "\n";
  }
};


class CausalPIBT : public Agent {
protected:
  // for tree structure
  CausalPIBT* parent;
  std::vector<CausalPIBT*> children;

  // priority
  Prio pori;
  Prio ptmp;

  // nodes
  Nodes C;
  Nodes S;

  virtual void init(Node* v, Node* g = nullptr);
  void priorityInheritance();
  void releaseChildren();
  void reset();
  virtual void actContracted();
  virtual void actRequesting();
  virtual void actExtended();

  Node* nextNode();
  CausalPIBT* chooseWinner(std::vector<CausalPIBT*> R);

public:
  CausalPIBT();
  ~CausalPIBT();

  void setParent(CausalPIBT* a);
  void addChild(CausalPIBT* a);
  void removeChild(CausalPIBT* a);
  Nodes getS() { return S; }
  Nodes getC() { return C; }
  void setS(Nodes _S) { S = _S; }
  void setC(Nodes _C) { C = _C; }
  Prio getPtmp() { return ptmp; }

  virtual bool isStable();
};
