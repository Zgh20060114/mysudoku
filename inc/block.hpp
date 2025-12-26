#pragma once

#include "color.hpp"
#include "common.hpp"
#include "displaySymbol.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <string>

// 3*3单元格/row/column
class Block {
private:
  static constexpr int GRID_COUNT{9};
  int stored_count{0};
  std::array<GridData *, GRID_COUNT> datas_of_num{};
  // TODO: 改成裸指针或者引用

public:
  Block() {}
  bool isValid() const {
    assert(stored_count == GRID_COUNT);
    for (int i = 0; i < GRID_COUNT - 1; ++i) {
      for (int j = i + 1; j < GRID_COUNT; ++j) {
        if (datas_of_num.at(i)->value == UNSELECTED ||
            datas_of_num.at(j)->value == UNSELECTED) {
          continue;
        }
        if (datas_of_num.at(i)->value == datas_of_num.at(j)->value) {
          return false;
        }
      }
    }
    return true;
  }

  bool isFull() const {
    for (int i = 0; i < GRID_COUNT; ++i) {
      if (datas_of_num.at(i)->value == UNSELECTED) {
        return false;
      }
    }
    return true;
  }

  // void resetData(int value) {
  //   for (auto &data : datas_of_num) {
  //     data = std::make_unique<GridData>(value, GridState::INITED);
  //   }
  // }

  void pushBack(GridData *gd) {
    if (gd) {
      datas_of_num[stored_count] = gd;
      stored_count++;
    }
  }

  int getValue(int n) {
    if (n < 0 || n > GRID_COUNT) {
      std::cerr << " n out of range!" << "\n";
    }
    return datas_of_num.at(n)->value;
  }

  void printPipeAndNum(int cur_pos, int highlight_num) const {
    // 先画第一列的，再画9列
    std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                 Color::AnsiCode::BG_DEFAULT,
                                 Color::AnsiCode::FG_RED)
              << Symbol::PIPE << Color::Modifier() << " ";
    for (int i = 0; i < stored_count; ++i) {
      auto &cur_data = datas_of_num[i];
      std::string data_s{" "};
      Color::AnsiCode pipe_fgcolor, num_bgcolor, num_fgcolor;
      if ((i + 1) % 3 == 0) {
        pipe_fgcolor = Color::AnsiCode::FG_RED;
      } else {
        pipe_fgcolor = Color::AnsiCode::FG_DEFAULT;
      }

      if (cur_data->state == GridState::ERASED) {
        num_fgcolor = Color::AnsiCode::FG_GREEN;
      } else {
        num_fgcolor = Color::AnsiCode::FG_DEFAULT;
      }

      if (i == cur_pos) {
        num_bgcolor = Color::AnsiCode::BG_BLUE;
      } else if (cur_data->value == highlight_num) {
        num_bgcolor = Color::AnsiCode::BG_YELLOW;
      } else {
        num_bgcolor = Color::AnsiCode::BG_DEFAULT;
      }

      if (cur_data->value != UNSELECTED) {
        data_s = std::to_string(cur_data->value);
      }

      std::cout << Color::Modifier(Color::AnsiCode::BOLD, num_bgcolor,
                                   num_fgcolor)
                << data_s << Color::Modifier() << " ";
      std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                   Color::AnsiCode::BG_DEFAULT, pipe_fgcolor)
                << Symbol::PIPE << Color::Modifier() << " ";
    }
    std::cout << "\n";
  }
};
