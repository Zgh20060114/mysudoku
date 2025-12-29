#pragma once

#include <algorithm>
#include <cstdio>
#include <fcntl.h>
#include <random>
#include <termios.h> //新
#include <vector>

inline constexpr unsigned int UNSELECTED = 0;

enum class Difficuity : int {
  EASY = 1,
  NORMAL,
  HARD,
};

enum class KeyMode : int {
  WASD = 1,
  VIM,
};

struct KeyMap {
  static constexpr char ESC{0x1B}; // 修饰成员变量，脱离类的实例，属于类本身
  static constexpr char U{0x75};
  static constexpr char ENTER{0x0D};
  char UP{};
  char DOWN{};
  char LEFT{};
  char RIGHT{};
};

struct Wasd : KeyMap {
  Wasd() {
    UP = 0x77;
    DOWN = 0x73;
    LEFT = 0x61;
    RIGHT = 0x64;
  }
};

struct Vim : KeyMap {
  Vim() {
    UP = 0x6b;
    DOWN = 0x6a;
    LEFT = 0x68;
    RIGHT = 0x6c;
  }
};

struct GridPos {
  int x; // 列
  int y; // 行
};

enum class GridState : int {
  INITED = 0,
  ERASED,
};

struct GridData {
  int value;
  GridState state;
  GridData(int value, GridState state) : value(value), state(state) {}
  GridData() {}
};

// 不用害怕返回一个很大的临时变量怎么办，编译器会自动解决：ROV（返回值优化），正常写就好了
inline std::vector<int> getUnit() {
  return std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
}

inline std::vector<int> getShuffleUnit() {
  std::vector<int> un = getUnit();
  std::random_device rd;
  std::mt19937 mt(rd());
  std::shuffle(un.begin(), un.end(), mt);
  return un;
}

inline int getRandomNumber(int begin, int end) {
  std::random_device rd; // 生成种子的设备
  std::mt19937 mt(rd()); // rd()生成一个种子，mt(种子)构造一个随机数生成引擎
  std::uniform_int_distribution<int> dist(begin, end);
  return dist(mt);
}

// 无行缓冲，无回显，传递原始字符
inline char getcharInstant() {
  termios t_attr, t_attr_raw;
  int fd = 0;
  tcgetattr(fd, &t_attr); // 得到目前默认的终端属性
  t_attr_raw = t_attr;
  cfmakeraw(&t_attr_raw);              // 构造原始属性
  tcsetattr(fd, TCSANOW, &t_attr_raw); // 设置原始属性给终端
  int c = std::getchar();
  if (c == 27) {
    auto t_flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, O_NONBLOCK | t_flags); // 暂时改为非阻塞模式
    c = std::getchar();
    if (c == EOF) {
      c = 27;
    } else {
      while ((c = std::getchar()) != EOF) {
        // 不管上下左右
      }
    }
    fcntl(fd, F_SETFL, t_flags); // 处理完了，改回正常模式
  }
  tcsetattr(fd, TCSANOW, &t_attr);
  return static_cast<char>(c);
}
