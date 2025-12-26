#include "../inc/block.hpp"
#include "../inc/scene.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  Block block;
  // block.resetData(3);
  // block.pushBack(1, GridState::INITED);
  // block.printPipeAndNum(6, 1);
  Scene scene;
  scene.generate9x9();
  scene.printAll();
  std::cout << "这是打印的，不是回显，" << getcharInstant() << "喵喵喵" << "\n";
  return 0;
}
