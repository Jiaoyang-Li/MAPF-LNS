/*
 * graph.cpp
 *
 * Purpose: Graph
 * Created by: Keisuke Okumura <okumura.k@coord.c.titech.ac.jp>
 */


#include "graph.h"
#include <random>
#include <unordered_set>
#include "util.h"


Graph::Graph() {
  std::random_device seed_gen;
  MT = new std::mt19937(seed_gen());
  init();
}

Graph::Graph(std::mt19937* _MT) : MT(_MT) {
  init();
}

void Graph::init() {
  regFlg = false;
}

Graph::~Graph() {
  for (auto v : nodes) delete v;
  nodes.clear();
  for (auto row : adjMatrix) row.clear();
  adjMatrix.clear();
  for (auto p : knownPaths) delete p.second;
  knownPaths.clear();
}

Node* Graph::getNode(int id) {
  auto itr = std::find_if(nodes.begin(), nodes.end(),
                          [id](Node* v){ return v->getId() == id; });
  // error check
  if (itr == nodes.end()) {
    std::cout << "error@Graph::getNode, "
              << "node index is over, " << id << "\n";
    std::exit(1);
  }

  return *itr;
}

bool Graph::existNode(int id) {
  auto itr = std::find_if(nodes.begin(), nodes.end(),
                          [id](Node* v){ return v->getId() == id; });
  return itr != nodes.end();
}

int Graph::getNodeIndex(Node* v) {
  return v->getIndex();
}

void Graph::setObj(Node* v) {
  objs.push_back(v->getId());

  int index = getNodeIndex(v);
  adjMatrix.erase(adjMatrix.begin() + index);

  std::vector<int>* row;
  for (int i = 0; i < adjMatrix.size(); ++i) {
    row = &adjMatrix[i];
    (*row).erase((*row).begin() + index);
  }
  nodes.erase(nodes.begin() + index);
  delete v;
}

void Graph::setObj(int id) {
  setObj(getNode(id));
}

void Graph::setObjs(std::vector<Node*> &objs) {
  for (auto v : objs) setObj(v);
}

void Graph::setObjs(std::vector<int> &objs) {
  for (auto v : objs) setObj(v);
}

std::vector<Node*> Graph::getNeighbor(Node* v) {
  int nodesNum = getNodesNum();
  int index = getNodeIndex(v);

  std::vector<Node*> N;
  for (int i = 0; i < nodesNum; ++i) {
    if (adjMatrix[index][i] == 1) N.push_back(nodes[i]);
  }
  return N;
}

void Graph::initNodes() {
  Node* v;
  for (int i = 0; i < nodes.size(); ++i) {
    v = nodes[i];
    v->setIndex(i);
  }
}

void Graph::setNeighbor() {
  Node* v;
  for (int i = 0; i < nodes.size(); ++i) {
    v = nodes[i];
    v->setNeighbor(getNeighbor(v));
  }
}

std::vector<Node*> Graph::neighbor(Node* v) {
  return v->getNeihbor();
}

std::vector<Node*> Graph::neighbor(int v) {
  return getNode(v)->getNeihbor();
}

struct AN {  // Astar Node
  Node* v;
  bool open;
  int cost;
  int f;
  AN* p;
};

std::vector<Node*> Graph::getPath(Node* s, Node* g,
                                  std::vector<Node*> &prohibitedNodes) {
  return {};
}

// regFlg : whether register path or not
std::vector<Node*> Graph::getPath(Node* _s, Node* _g,
                                  std::vector<Node*> &prohibitedNodes,
                                  int (*dist) (Node*, Node*))
{
  bool prohibited = !prohibitedNodes.empty();
  std::vector<Node*> path, C;
  std::string key;

  // known path or not
  if (regFlg && !prohibited) {
    key = getKeyForKnownPath(_s, _g);
    auto itrK = knownPaths.find(key);
    if (itrK != knownPaths.end()) {  // known
      path = itrK->second->path;
      return path;
    }
  }

  // prepare node open hashtable
  std::unordered_map<int, AN*> table;
  AN* t = new AN { _s, true, 0, dist(_s, _g), nullptr };
  table.emplace(getNodeIndex(_s), t);

  struct AN* n = nullptr;
  struct AN* l = nullptr;
  int f, index;

  std::unordered_set<int> OPEN = { getNodeIndex(_s) };  // ID list

  while (!OPEN.empty()) {
    // argmin
    auto itr = std::min_element(OPEN.begin(), OPEN.end(),
                                [&table] (int a, int b)
                                { auto eleA = table.at(a);
                                  auto eleB = table.at(b);
                                  if (eleA->f == eleB->f) {
                                    return eleA->cost > eleB->cost;
                                  }
                                  return eleA->f < eleB->f; });

    index = *itr;
    n = table.at(index);

    // check goal condition
    if (n->v == _g) break;

    // already known?
    key = getKeyForKnownPath(n->v, _g);
    auto itrK = knownPaths.find(key);
    if (itrK != knownPaths.end()) {  // known
      std::vector<Node*> kPath = itrK->second->path;
      bool valid = true;
      if (prohibited) {
        for (auto v : kPath) {
          if (inArray(v, prohibitedNodes)) {
            valid = false;
            break;
          }
        }
      }
      if (valid) {
        for (int i = 1; i < kPath.size(); ++i) {
          n = new AN { kPath[i], true, 0, 0, n };
        }
        break;
      }
    }


    // update list
    n->open = false;
    OPEN.erase(itr);

    // search neighbor
    C = neighbor(n->v);

    for (auto m : C) {
      if (prohibited && inArray(m, prohibitedNodes)) continue;
      index = getNodeIndex(m);

      auto itr = table.find(index);
      if (itr == table.end()) {
        AN* t = new AN { m, true, 0, 100000, nullptr };
        table.emplace(index, t);
      }
      l = table.at(index);
      if (!l->open) continue;

      f = n->cost + dist(m, _g);
      if (regFlg) {
        key = getKeyForKnownPath(m, _g);
        auto itrK = knownPaths.find(key);
        if (itrK != knownPaths.end()) {
          f = n->cost + itrK->second->path.size() - 1;
        }
      }

      if (l->f > f) {
        l->cost = n->cost + 1;
        l->f = f;
        l->p = n;
      }
      OPEN.insert(index);
    }
  }

  // back tracking
  while (n->p) {
    path.push_back(n->v);
    n = n->p;
  }
  path.push_back(_s);
  std::reverse(path.begin(), path.end());

  // register path
  if (regFlg && !prohibited) registerPath(path);

  return path;
}

std::string Graph::getKeyForKnownPath(Node* s, Node* g) {
  int sIndex = getNodeIndex(s);
  int gIndex = getNodeIndex(g);
  std::string key = "";
  key += std::to_string(sIndex);
  key += "-";
  key += std::to_string(gIndex);
  return key;
}

void Graph::registerPath(const std::vector<Node*> &path) {
  if (path.empty()) return;

  std::vector<Node*> tmp = path;
  std::string key;

  Node *v1, *v2;
  do {
    v1 = tmp[0];
    v2 = tmp[tmp.size() - 1];
    key = getKeyForKnownPath(v1, v2);
    KnownPath* knownPath = new KnownPath { v1, v2, tmp };
    knownPaths.emplace(key, knownPath);
    tmp.erase(tmp.begin());
  } while (tmp.size() > 2);
}

std::vector<std::vector<Node*>> Graph::getStartGoal(int num) {
  if (num > nodes.size()) {
    std::cout << "error@Graph::getStartGoal, over node size" << "\n";
    std::exit(1);
  }

  std::vector<std::vector<Node*>> points;
  std::vector<Node*> starts(nodes.size());
  std::vector<Node*> goals(nodes.size());
  bool flg;

  std::copy(nodes.begin(), nodes.end(), starts.begin());
  std::copy(nodes.begin(), nodes.end(), goals.begin());

  while (true) {
    points.clear();
    std::shuffle(starts.begin(), starts.end(), *MT);
    std::shuffle(goals.begin(), goals.end(), *MT);

    flg = true;
    for (int i = 0; i < num; ++i) {
      if (starts[i] != goals[i]) {
        points.push_back({ starts[i], goals[i] });
      } else {
        flg = false;
        break;
      }
    }

    if (flg) break;
  }

  return points;
}

std::string Graph::logStr() {
  std::string str;
  str += "[graph] width:" + std::to_string(getW()) + "\n";
  str += "[graph] height:" + std::to_string(getH()) + "\n";
  str += "[graph] objs:";
  for (auto i : objs) str += std::to_string(i) + ",";
  return str;
}
