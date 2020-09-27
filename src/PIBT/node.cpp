/*
 * node.cpp
 *
 * Purpose: Node
 * Created by: Keisuke Okumura <okumura.k@coord.c.titech.ac.jp>
 */


#include "node.h"

int Node::cntId = 0;


Node::Node() : id(cntId) {
  ++cntId;
  pos = Vec2f(0, 0);
}

Node::Node(int _id) : id(_id) {
  cntId = id + 1;
  pos = Vec2f(0, 0);
}
