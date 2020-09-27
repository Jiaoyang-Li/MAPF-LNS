#pragma once


#include "grid.h"
#include <string>

// map for Dragon Age: Origins
class DAO : public Grid {
private:
  void init();

protected:
  std::string filename;

public:
  DAO(std::string _filename);
  DAO(std::string _filename, std::mt19937* _MT);
  ~DAO();

  std::string logStr();
};
