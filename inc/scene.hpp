#pragma once

#include "block.hpp"
#include "common.hpp"
#include "displaySymbol.hpp"
#include "operate.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility> // pair
#include <vector>

class Scene {
private:
  std::array<GridData, 81> grids{};
  GridPos cur_cursor_pos{};
  std::array<Block, 9> column_blocks{};
  std::array<Block, 9> row_blocks{};
  std::array<std::array<Block, 3>, 3> subgrids;
  std::unique_ptr<KeyMap> key_map{};
  std::vector<Operate> operate_vec{};

  bool isGenerateOrPlayComplete() {
    for (auto &block : column_blocks) {
      if (!block.isFull() || !block.isValid()) {
        return false;
      }
    }
    for (auto &block : row_blocks) {
      if (!block.isValid()) {
        return false;
      }
    }
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (!subgrids[i][j].isValid()) {
          return false;
        }
      }
    }
    return true;
  }

  void init() {
    for (int i = 0; i < 9; ++i) { // 列
      Block &column_block = column_blocks[i];
      for (int j = 0; j < 9; ++j) { // 行
        column_block.pushBack(grids.data() + j * 9 + i);
      }
    }
    for (int i = 0; i < 9; ++i) { // 行
      Block &row_block = row_blocks[i];
      for (int j = 0; j < 9; ++j) { // 行
        row_block.pushBack(grids.data() + i * 9 + j);
      }
    }
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        subgrids[i / 3][j / 3].pushBack(grids.data() + i * 9 + j);
      }
    }
  }

  void setGridValue(const GridPos &xy, int value) {
    grids[xy.x + 9 * xy.y].value = value;
  }

public:
  Scene() : cur_cursor_pos({0, 0}) { init(); }

  GridPos getCurCursorPos() { return cur_cursor_pos; }

  bool setCurPosValue(GridPos pos, int value) {
    auto &cur_grid = grids.at(pos.x + pos.y * 9);
    if (cur_grid.state != GridState::ERASED) {
      return false;
    } else {
      cur_grid.value = value;
    }
    return true;
  }

  int getCurPosValue(GridPos pos) {
    auto &cur_grid = grids.at(pos.x + pos.y * 9);
    return cur_grid.value;
  }

  void randomErase(int n) {
    std::vector<int> pool(81);
    for (int i = 0; i < 81; ++i) {
      pool[i] = i;
    }
    for (int i = 0; i < n; ++i) {
      int r = getRandomNumber(0, pool.size() - 1);
      grids[pool.at(r)] = GridData(UNSELECTED, GridState::ERASED);
      pool.erase(pool.begin() + r); // 会往前递补
    }
  }

  void printAll() {
    system("clear");
    printCorneAndLine(-1);
    int highlight_num = row_blocks[cur_cursor_pos.y].getValue(cur_cursor_pos.x);
    for (int i = 0; i < 9; ++i) {
      auto &row_block_i = row_blocks.at(i);
      if (cur_cursor_pos.y == i) {
        row_block_i.printPipeAndNum(cur_cursor_pos.x, highlight_num);
      } else {
        row_block_i.printPipeAndNum(-1, highlight_num);
      }
      printCorneAndLine(i);
    }
  }

  void setKeyMap(KeyMode mode) {
    if (mode == KeyMode::WASD) {
      key_map = std::make_unique<Wasd>();
    } else if (mode == KeyMode::VIM) {
      key_map = std::make_unique<Vim>();
    }
  } // cs106l里讲的，实现多态。不用指针，将派生类对象赋值给基类对象将导致切片

  void play() {
    printAll();
    while (1) {
      char key = getcharInstant();
      std::cout << ":key = " << static_cast<int>(key) << "\n";
      if (key >= '0' && key <= '9') {
        Operate operate{this};
        if (!operate.execute(key - '0')) {
          std::cerr << "can't be modified! " << "\n";
        } else {
          operate_vec.push_back(operate);
          printAll();
        }
      } else if (key == key_map->ESC) {
        std::cout << "leave this game ?" << "\n";
        std::string in_str{};
        std::cin >> in_str;
        if (in_str[0] == 'y' || in_str[0] == 'Y') {
          // exit(0);
          break;
        } else if (in_str[0] == 'n' || in_str[0] == 'N') {
          printAll();
        }
      } else if (key == key_map->U) {
        if (operate_vec.size() == 0) {
          std::cerr << "can't undo !" << "\n";
        } else {
          auto &cur_operate = operate_vec.back();
          cur_operate.undo();
          operate_vec.pop_back();
          printAll();
        }
      } else if (key == key_map->LEFT) {
        cur_cursor_pos.x =
            (cur_cursor_pos.x - 1) > 0 ? (cur_cursor_pos.x - 1) : 0;
        printAll();
      } else if (key == key_map->RIGHT) {
        cur_cursor_pos.x =
            (cur_cursor_pos.x + 1) < 8 ? (cur_cursor_pos.x + 1) : 8;
        printAll();
      } else if (key == key_map->UP) {
        cur_cursor_pos.y =
            (cur_cursor_pos.y - 1) > 0 ? (cur_cursor_pos.y - 1) : 0;
        printAll();
      } else if (key == key_map->DOWN) {
        cur_cursor_pos.y =
            (cur_cursor_pos.y + 1) < 8 ? (cur_cursor_pos.y + 1) : 8;
        printAll();
      } else if (key == key_map->ENTER) {
        if (isGenerateOrPlayComplete()) {
          std::cout << "Win !" << "\n";
          break;
        } else {
          std::cout << "Continue !" << "\n";
        }
      }
    }
  }

  void printCorneAndLine(int line_num) {
    for (int i = 0; i < 9; ++i) {
      // corner
      // 3*3的底边 || 3*3的侧边 || 顶行
      if ((line_num + 1) % 3 == 0 || i % 3 == 0 || line_num == -1) {
        std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                     Color::AnsiCode::BG_DEFAULT,
                                     Color::AnsiCode::FG_RED)
                  << Symbol::CORNER;
      } else {
        std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                     Color::AnsiCode::BG_DEFAULT,
                                     Color::AnsiCode::FG_DEFAULT)
                  << Symbol::CORNER;
      }
      // line
      auto line_or_arrow =
          (cur_cursor_pos.y == line_num && cur_cursor_pos.x == i)
              ? Symbol::ARROW
              : Symbol::LINE;
      if (line_num == -1 || (line_num + 1) % 3 == 0) {
        std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                     Color::AnsiCode::BG_DEFAULT,
                                     Color::AnsiCode::FG_RED)
                  << Symbol::LINE << line_or_arrow << Symbol::LINE;
      } else {
        std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                     Color::AnsiCode::BG_DEFAULT,
                                     Color::AnsiCode::FG_DEFAULT)
                  << Symbol::LINE << line_or_arrow << Symbol::LINE;
      }
    }
    std::cout << Color::Modifier(Color::AnsiCode::BOLD,
                                 Color::AnsiCode::BG_DEFAULT,
                                 Color::AnsiCode::FG_RED)
              << Symbol::CORNER;
    std::cout << "\n";
  }
  void generate9x9() {
    std::vector<std::vector<int>> matrix{};
    for (int i = 0; i < 9; ++i) {
      matrix.push_back(std::vector<int>(9, 0));
    }
    // 先给对角线方块赋值，让每行每列都有数字
    for (int i = 0; i < 3; ++i) {
      std::vector<int> sunit = getShuffleUnit();
      for (int j = i * 3; j < i * 3 + 3; ++j) {
        for (int k = i * 3; k < i * 3 + 3; ++k) {
          matrix[j][k] = sunit.back();
          sunit.pop_back();
        }
      }
    }
    // for (int i = 0; i < 9; ++i) {
    //   for (int j = 0; j < 9; ++j) {
    //     std::cout << matrix[i][j] << " , ";
    //   }
    //   std::cout << "\n";
    // }
    // std::cout << "\n";
    // std::cout << "\n";

    // 统计空格位置
    std::vector<std::pair<int, int>> blank_poss{};
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        if (matrix[i][j] == 0) {
          blank_poss.push_back(std::pair<int, int>{i, j});
        }
      }
    }
    // 填充空格
    std::map<std::string, std::vector<int>> avaliable_num{};
    std::size_t blank_size = blank_poss.size();
    int full_size{};
    std::string avaliable_key{};
    while (full_size < blank_size) {
      int x = blank_poss[full_size].first;
      int y = blank_poss[full_size].second;
      avaliable_key = std::to_string(x) + "x" + std::to_string(y);
      std::vector<int> avali_unit{};
      // 填充avaliable_num
      if (avaliable_num.find(avaliable_key) == avaliable_num.end()) {
        avali_unit = getUnit();
        // 0-2, 3-5, 6-8
        // (x,y)所在的3*3
        for (int i = x / 3 * 3; i < x / 3 * 3 + 3; ++i) {
          for (int j = y / 3 * 3; j < y / 3 * 3 + 3; ++j) {
            avali_unit.erase(
                std::remove(avali_unit.begin(), avali_unit.end(), matrix[i][j]),
                avali_unit.end());
          }
        }
        // 行
        for (int i = 0; i < 9; ++i) {
          if (matrix[i][y] != 0) {
            avali_unit.erase(
                std::remove(avali_unit.begin(), avali_unit.end(), matrix[i][y]),
                avali_unit.end());
          }
        }
        // 列
        for (int i = 0; i < 9; ++i) {
          if (matrix[x][i]) {
            avali_unit.erase(
                std::remove(avali_unit.begin(), avali_unit.end(), matrix[x][i]),
                avali_unit.end());
          }
        }
        avaliable_num[avaliable_key] = avali_unit;
      }

      if (avaliable_num[avaliable_key].size() > 0) {
        matrix[x][y] = avaliable_num[avaliable_key].back();
        avaliable_num[avaliable_key].pop_back();
        full_size++;
      } else {
        avaliable_num.erase(avaliable_key);
        matrix[x][y] = 0; // 第一层回溯的时候清不清零无所谓，因为还没赋值；
        // 但是第二层的时候已经被赋上错误的值了，就必须清零了
        full_size--;
        if (full_size == -1) {
          std::cerr << "generate error , again !" << "\n";
        }
      }
    }

    // 填入grids
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        setGridValue(GridPos{i, j}, matrix[i][j]);
        std::cout << matrix[i][j] << " , ";
      }
      std::cout << "\n";
    }

    assert(isGenerateOrPlayComplete());
  }
};
