/*
 * pps.h
 *
 * Purpose: Parallel Push & Swap
 * Created by: Keisuke Okumura <okumura.k@coord.c.titech.ac.jp>
 */

#pragma once
#include "solver.h"


enum RES { SUCCESS,
           FAIL,
           PAUSE };
enum CHECK { VALID, INVALID };
enum SWAPPHASE { GO_TARGET,
                 CLEARING,
                 EVAC_H,
                 EVAC_L,
                 SWAP_DONE };

struct S {
  int id;
  AGENTS agents;  // priority : high priority, low priority
  Node* lowOriginalNode;  // [low] original pos, [high] goal pos
  NODES esv;  // swap node candidates
  Node* target;  // swap node, deg(target) >= 3
  Node* origin;  // low pos when swap starts
  Node* evacH;  // [high] evacuation node
  Node* evacL;  // [low]  evacuation node
  NODES area;   // swap area, using case 3
  SWAPPHASE phase;  // phase
};

class PPS : public Solver {
private:
  AGENTS pushers;  // pushers
  std::vector<S*> swapers;   // swapers
  AGENTS pusherToSwaper;  // tmp
  AGENTS swaperToPusher;  // tmp
  std::vector<S*> doneSwapers;  // tmp

  AGENTS M;  // moved agents
  AGENTS U;  // goal agents
  NODES L;   // reserved nodes
  bool status;  // continue or not

  NODES goals;  // final goal
  std::vector<bool> isTmpGoals;  // has temp goal
  NODES deg3nodes;

  static int s_uuid;

  void init();

  RES SWAP(S* s);

  RES PUSH(PIBT_Agent* c, bool swap);
  RES PUSH(PIBT_Agent* c, NODES &T, bool swap);
  RES PUSH(PIBT_Agent* c, AGENTS &H, bool swap);
  RES PUSH(PIBT_Agent* c, AGENTS &H, NODES &T, bool swap);
  RES PUSH(S* s);

  RES FEASIBLE(PIBT_Agent* c, NODES &pi, AGENTS& H, NODES& T, bool swap);
  RES FEASIBLE(S* s, NODES &pi);

  NODES SHORTEST_PATH(Node* s, Node* g);
  NODES SHORTEST_PATH(PIBT_Agent* c, Node* g);
  NODES SHORTEST_PATH(PIBT_Agent* c, Node* g, AGENTS& H);
  NODES SHORTEST_PATH(Node* s, Node* g, NODES prohibited);
  NODES SHORTEST_PATH(PIBT_Agent* c, Node* g, AGENTS& H, NODES& T);

  bool DEPEND(NODES piA, NODES piB);

  void SETUP_SWAP(PIBT_Agent* c, PIBT_Agent* a);

  bool CLEAR(S* s);

  void SWAP_PRIMITIVES(S* s);
  void FINISH_SWAP(S* s);

  void move(PIBT_Agent* a, NODES &pi);
  void move(S* s, NODES &pi);

  bool reserved(Node* v, AGENTS &M);
  bool isFree(Node* v);
  bool inS(PIBT_Agent* a);
  S* getS(PIBT_Agent* a);
  NODES getSortedEsv(PIBT_Agent* c);

  CHECK CHECK_PRIORITY(S* s, PIBT_Agent* a);
  CHECK CHECK_PRIORITY(PIBT_Agent* c, PIBT_Agent* a);
  CHECK CHECK_PRIORITY(S* sC, S* sA);

  RES FIND_NEW_VERTEX(S* s);

  CHECK CHECK_PUSHER(PIBT_Agent* a);
  CHECK CHECK_SWAPER(S* s);
  CHECK CHECK_GOAL(PIBT_Agent* a);

  void ADD_DONE_SWAPERS(S* s);
  NODES CLOSEST_EMPTY_VERTICLES(PIBT_Agent* c);

public:
  PPS(Problem* _P);
  PPS(Problem* _P, std::mt19937* _MT);
  ~PPS();

  bool solve();
  void update();

  std::string logStr();
};
