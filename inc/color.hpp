#pragma once
#include <ostream>

namespace Color {
enum class AnsiCode : int {
  BOLD = 1,
  RESET = 0,

  BG_DEFAULT = 49,
  BG_BLACK = 40,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_YELLOW = 43,
  BG_BLUE = 44,
  BG_MAGENTA = 45,
  BG_CYAN = 46,
  BG_LIGHT_GRAY = 47, // 实际是白色，终端为正常显示会进行调整

  FG_DEFAULT = 39,
  FG_BLACK = 30,
  FG_RED = 31,
  FG_GREEN = 32,
  FG_YELLOW = 33,
  FG_BLUE = 34,
  FG_MAGENTA = 35,
  FG_CYAN = 36,
  FG_LIGHT_GRAY = 37,
  FG_DARK_GRAY = 90,
  FG_LIGHT_RED = 91,
  FG_LIGHT_GREEN = 92,
  FG_LIGHT_YELLOW = 93,
  FG_LIGHT_BLUE = 94,
  FG_LIGHT_MAGENTA = 95,
  FG_LIGHT_CYAN = 96,
  FG_WHITE = 97,
};

inline std::ostream &operator<<(std::ostream &os, const AnsiCode &code) {
  return os << static_cast<int>(code);
}
class Modifier {
private:
  AnsiCode md, bg, fg;

public:
  Modifier()
      : md(AnsiCode::RESET), bg(AnsiCode::BG_DEFAULT),
        fg(AnsiCode::FG_DEFAULT) {}
  Modifier(AnsiCode m, AnsiCode b, AnsiCode f) : md(m), bg(b), fg(f) {}
  friend std::ostream &operator<<(std::ostream &os, const Modifier &modfier) {
    return os << "\033[" << modfier.md << ";" << modfier.bg << ";" << modfier.fg
              << "m";
  }
};
}; // namespace Color
