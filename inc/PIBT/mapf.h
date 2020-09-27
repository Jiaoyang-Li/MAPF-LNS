#pragma once
#include "problem.h"


class MAPF : public Problem {
private:
  void init();

public:
  MAPF(Graph* _G,
       std::vector<PIBT_Agent*> _A,
       std::vector<Task*> _T);
  MAPF(Graph* _G,
       std::vector<PIBT_Agent*> _A,
       std::vector<Task*> _T,
       std::mt19937* _MT);
  ~MAPF();
  bool isSolved();
  void update();
  bool allocated() { return true; }

  std::string logStr();
};
