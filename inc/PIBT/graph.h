#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include "node.h"

// cash calculated path
struct KnownPath {
  Node* s;
  Node* g;
  std::vector<Node*> path;
};

class Graph {
private:
  std::vector<Node*> getNeighbor(Node* v);

protected:
  // nodes
  std::vector<Node*> nodes;
  // adjacency matrix
  std::vector<std::vector<int>> adjMatrix;
  // objs
  std::vector<int> objs;
  // register path or not
  bool regFlg;
  // searched path
  std::unordered_map<std::string, KnownPath*> knownPaths;
  // random generator
  std::mt19937* MT;

  void init();
  virtual void initNodes();
  void setNeighbor();
  std::vector<Node*> getPath(Node* s, Node* g, int (*dist)(Node*, Node*));
  std::vector<Node*> getPath(Node* s, Node* g,
                             std::vector<Node*> &prohibitedNodes,
                             int (*dist)(Node*, Node*));
  std::string getKeyForKnownPath(Node* s, Node* g);
  void registerPath(const std::vector<Node*> &path);


public:
  Graph();
  Graph(std::mt19937* _MT);
  ~Graph();

  Node* getNode(int id);
  bool existNode(int id);
  int getNodesNum() { return nodes.size(); }
  int getNodeIndex(Node* v);
  Node* getNodeFromIndex(int i) { return nodes[i]; }

  void setObj(Node* v);
  void setObj(int id);
  void setObjs(std::vector<Node*> &objs);
  void setObjs(std::vector<int> &ids);

  std::vector<Node*> neighbor(Node* v);
  std::vector<Node*> neighbor(int v);

  virtual int getW() { return 0; };
  virtual int getH() { return 0; };

  void setRegFlg(bool flg) { regFlg = flg; }

  std::vector<Node*> getNodes() { return nodes; }

  virtual int dist(Node* v1, Node* v2) { return 0; }
  virtual std::vector<Node*> getPath(Node* s, Node* g) { return {}; }
  virtual std::vector<Node*> getPath(Node* s, Node* g,
                                     std::vector<Node*> &prohibitedNodes);

  virtual std::vector<std::vector<Node*>> getStartGoal(int num);

  // for MAPD
  virtual std::vector<Node*> getPickup() { return {}; }
  virtual std::vector<Node*> getDelivery() { return {}; }
  virtual std::vector<Node*> getEndpoints() { return {}; }
  virtual std::vector<Node*> getAllSpecialPoints() { return {}; }

  virtual std::string logStr();
};
