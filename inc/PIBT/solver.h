#pragma once

#include "problem.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <eigen3/Eigen/Core>
#include <unordered_map>
#include <unordered_set>

typedef std::chrono::duration<float> fsec;

using AGENTS = std::vector<PIBT_Agent*>;
using NODES = std::vector<Node*>;
using Paths = std::vector<std::vector<Node*>>;

struct AN {  // for A* search
  Node* v;
  bool open;
  int t;
  int f;
  AN* p;
};

class Solver {
private:
  double elapsedTime;  // runtime


protected:
  Problem* P;
  std::mt19937* MT;

  std::vector<PIBT_Agent*> A;
  Graph* G;

  Eigen::MatrixXi dists;  // path dist

  void init();
  int getMaxLengthPaths(Paths& paths);  // get max length within paths
  void formalizePath(Paths& paths);  // formalize paths with different lengths
  int pathDist(Node* v, Node* u);
  int pathDist(Node* s, Node* g, std::vector<Node*> &prohibited);  // with some prohibited nodes
  static std::string getKey(int t, Node* v);

  virtual void solveStart();
  virtual void solveEnd();
  std::chrono::system_clock::time_point startT;
  std::chrono::system_clock::time_point endT;
  double time_limit = 0;

public:
  Solver(Problem* _P);
  Solver(Problem* _P, std::mt19937* _MT);
  ~Solver();

  void WarshallFloyd();
  void setTimeLimit(double limit){this->time_limit = limit;};


    virtual bool solve() { return false; };
  double getElapsed() { return elapsedTime; };

  virtual std::string logStr();
};
