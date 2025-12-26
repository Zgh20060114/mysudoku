#pragma once
#include "common.hpp"

class Scene;
class Operate {
private:
  Scene *cur_owner;
  GridPos cur_pos;
  int per_value;
  int cur_value;

public:
  // Operate(const Operate &other_ope) {
  //   cur_owner = other_ope.cur_owner;
  //   cur_pos = other_ope.cur_pos;
  //   per_value = other_ope.per_value;
  //   cur_value = other_ope.cur_value;
  // }
  Operate(Scene *ownwer);
  bool execute(int writing_value);
  bool undo();
};
