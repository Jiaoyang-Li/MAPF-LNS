/*
 * Grid.cpp
 *
 * Purpose: Grid
 * Created by: Keisuke Okumura <okumura.k@coord.c.titech.ac.jp>
 */

#include "grid.h"


Grid::Grid(std::mt19937* _MT) : Graph(_MT) {}


Grid::Grid(int _w, int _h) : w(_w), h(_h) {
  setGrid();
}

Grid::Grid(int _w, int _h, std::vector<int> objs) : w(_w), h(_h) {
  setGrid();
  for (auto obj : objs) setObj(obj);
}

Grid::Grid() {};
Grid::~Grid() {}


void Grid::setSize(int _w, int _h) {
  w = _w;
  h = _h;
}


void Grid::setGrid() {
  // initialize
  for (auto v : nodes) delete v;
  nodes.clear();
  adjMatrix.clear();

  int nodesNum = w * h;
  int index;

  // set grid
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      index = j * w + i;
      std::vector<int> row(nodesNum, 0);
      if (i != 0) row[index - 1] = 1;
      if (i != w - 1) row[index + 1] = 1;
      if (j != 0) row[index - w] = 1;
      if (j != h - 1) row[index + w] = 1;
      adjMatrix.push_back(row);
      Node* node = new Node();
      node->setPos(i, j);
      nodes.push_back(node);
    }
  }
}

int Grid::manhattanDist(Node* v, Node* u) {
  Vec2f vPos = v->getPos();
  Vec2f uPos = u->getPos();
  return (int)(std::abs((int)(vPos.x - uPos.x))
               + std::abs((int)(vPos.y - uPos.y)));
}

int Grid::manhattanDist(int v, int u) {
  return manhattanDist(nodes[v], nodes[u]);
}

std::vector<Node*> Grid::getPath(Node* s, Node* g) {
  std::vector<Node*> nodes = {};
  return Graph::getPath(s, g, nodes, manhattanDist);
}

std::vector<Node*> Grid::getPath(Node* s, Node* g,
                                 std::vector<Node*> &prohibitedNodes) {
  return Graph::getPath(s, g, prohibitedNodes, manhattanDist);
}

std::vector<Node*> Grid::getPath(int s, int g) {
  return getPath(getNode(s), getNode(g));
}

void Grid::initNodes() {
  // create adjmatrix
  int nodesNum = nodes.size();
  int kd = 0;
  int ku = 0;
  int l, id;
  Node* v;

  adjMatrix.clear();
  for (int index = 0; index < nodesNum; ++index) {
    v = nodes[index];
    id = v->getId();

    std::vector<int> row(nodesNum, 0);
    // left
    if (index > 0 && v->getPos().x > 0 &&
        nodes[index-1]->getPos().y == v->getPos().y &&
        std::abs((int)(nodes[index-1]->getPos().x - v->getPos().x)) <= 1) {
      row[index - 1] = 1;
    }
    // right
    if (index < nodesNum - 1 && v->getPos().x < w - 1 &&
        nodes[index+1]->getPos().y == v->getPos().y &&
        std::abs((int)(nodes[index+1]->getPos().x - v->getPos().x)) <= 1) {
      row[index + 1] = 1;
    }
    // down
    if (v->getPos().y > 0) {
      if (objs.empty()) {
        row[index - w] = 1;
      } else {
        while (true) {
          if (kd >= objs.size() - 1) break;
          if (kd < objs.size() - 1 && objs[kd+1] > id - w) break;
          ++kd;
        }

        if (objs[kd] != id - w) {
          l = kd;
          while (true) {
            if (l >= objs.size() - 1) break;
            if (l < objs.size() - 1 && objs[l+1] >= id) break;
            ++l;
          }

          if (id < objs[kd] || objs[l] < id - w) {
            row[index - w] = 1;
          } else if (objs[kd] > id - w) {
            row[index - w + (l - kd) + 1] = 1;
          } else {
            row[index - w + (l - kd)] = 1;
          }
        }
      }
    }
    // up
    if (v->getPos().y < h - 1) {
      if (objs.empty()) {
        row[index + w] = 1;
      } else {
        while (true) {
          if (ku >= objs.size() - 1) break;
          if (ku < objs.size() - 1 && objs[ku+1] > id + w) break;
          ++ku;
        }

        if (objs[ku] != id + w) {
          l = ku;
          while (true) {
            if (l <= 0) break;
            if (l > 0 && objs[l-1] < id) break;
            --l;
          }

          if (objs[ku] < id) {
            row[index + w] = 1;
          } else if (objs[ku] > id + w) {
            row[index + w] = 1;
          } else {
            row[index + w - (ku - l + 1)] = 1;
          }
        }
      }
    }
    adjMatrix.push_back(row);
  }

  Graph::initNodes();
  Graph::setNeighbor();
  adjMatrix.clear();
  //std::cout << "finish creating map" << "\n";
}
