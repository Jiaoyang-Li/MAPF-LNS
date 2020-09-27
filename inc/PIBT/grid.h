#pragma once

#include <iostream>

#include "graph.h"


class Grid : public Graph {
protected:
  int w;
  int h;
  void setSize(int _w, int _h);
  void setGrid();
  void initNodes();

public:
  Grid();
  Grid(std::mt19937* _MT);
  Grid(int w, int h);
  Grid(int w, int h, std::vector<int> objs);
  ~Grid();

  int getW() { return w; }
  int getH() { return h; }

  static int manhattanDist(Node* v, Node* u);
  int manhattanDist(int v, int u);

  std::vector<Node*> getPath(Node* s, Node* g);
  std::vector<Node*> getPath(Node* s, Node* g,
                             std::vector<Node*> &prohibitedNodes);
  std::vector<Node*> getPath(int s, int g);

  int dist(Node* v1, Node* v2) { return manhattanDist(v1, v2); }
};
