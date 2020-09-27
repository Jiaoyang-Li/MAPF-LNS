/*
 * denmap.cpp
 *
 * Purpose: DAO Map
 * Created by: Keisuke Okumura <okumura.k@coord.c.titech.ac.jp>
 */

#include <fstream>
#include <regex>
#include "dao.h"


DAO::DAO(std::string _filename, std::mt19937* _MT)
  : Grid(_MT), filename(_filename)
{
  init();
}

DAO::DAO(std::string _filename) : filename(_filename) {
  init();
}

void DAO::init() {
  std::ifstream file(filename);
  if (!file) {
    std::cout << "error@DAO::init, file "
              << filename
              << " does not exist" << "\n";
    std::exit(1);
  }

  std::string line;
  std::smatch results;
  int w  = 0;
  int h = 0;
  bool mapStart = false;

  std::regex r_height = std::regex(R"(height\s(\d+))");
  std::regex r_width = std::regex(R"(width\s(\d+))");
  std::regex r_map = std::regex(R"(map)");

  int j = 0;
  int index = 0;
  nodes.clear();
  objs.clear();

  while (getline(file, line)) {
    if (mapStart) {
      for (int i = 0; i < w; ++i) {
        index = j * w + i;
        if (line[i] == 'T' or line[i] == '@') {
          objs.push_back(index);
        } else {
          Node* v = new Node(index);
          v->setPos(i, j);
          nodes.push_back(v);
        }
      }
      ++j;
    }

    if (std::regex_match(line, results, r_height)) {
      h = std::stoi(results[1].str());
    }

    if (std::regex_match(line, results, r_width)) {
      w = std::stoi(results[1].str());
    }

    if (std::regex_match(line, results, r_map)) {
      mapStart = true;
      setSize(w, h);
    }
  }
  file.close();

  initNodes();
}

DAO::~DAO() {}


std::string DAO::logStr() {
  std::string str;
  str += "[graph] file:" + filename + "\n";
  str += Graph::logStr();
  return str;
}
