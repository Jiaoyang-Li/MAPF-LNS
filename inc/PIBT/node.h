#pragma once

#include "vec2f.h"
#include <iostream>
#include <vector>


class Node {
private:
  Vec2f pos;
  const int id;  // notice: id is not index!
  int index;
  std::vector<Node*> neighbor;

  static int cntId;  // for uuid

public:
  Node();
  Node(int _id);
  ~Node() {};

  std::vector<Node*> getNeihbor() { return neighbor; }
  void setNeighbor(std::vector<Node*> nodes) { neighbor = nodes; }

  Vec2f getPos() { return pos; }
  void  setPos(Vec2f _pos) { pos = _pos; }
  void  setPos(int x, int y) { pos = Vec2f(x, y); }

  void setIndex(int i) { index = i; }
  int getIndex() { return index; }

  int getId() { return id; }

  bool operator==(Node* v) const { return v->getId() == id; };
  bool operator!=(Node* v) const { return v->getId() != id; };
  bool operator==(Node& v) const { return v.getId() == id; };
  bool operator!=(Node& v) const { return v.getId() != id; };
};
