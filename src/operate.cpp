#include "../inc/operate.hpp"
#include "../inc/scene.hpp"

bool Operate::execute(int writing_value) {
  cur_pos = cur_owner->getCurCursorPos();
  per_value = cur_owner->getCurPosValue(cur_pos);
  return cur_owner->setCurPosValue(cur_pos, writing_value);
}

bool Operate::undo() { return cur_owner->setCurPosValue(cur_pos, per_value); }

Operate::Operate(Scene *owner) : cur_owner(owner) {}
