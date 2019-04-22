#ifndef _BOX_H
#define _BOX_H

#include <windows.h>
#include <conio.h>
#include <array>

using std::array;
using std::pair;

array<array<int, 6>, 7> bitmap{
  {-1, 0, 1, 0, 2, 0},  // I
  // {-1, 0, 1, 0, 1, -1}, // J
  // {-1, 0, 1, 0, 1, 1},  // L
  // {0, 1, 1, 0, 1, 1},   // O
  // {0, 1, 1, 0, 1, -1},  // S
  // {1, 0, 0, 1, 0, -1},  // T
  // {0, -1, 1, 0, 1, 1},  // Z
};

typedef array<array<int, 12>, 23> Map;
typedef pair<int, int> Position;

void set_pos(short x, short y) {
  COORD pos{x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

void reset_pos() {
  set_pos(0, 0);
}

enum Shape {
  shapeI, 
  shapeJ,
  shapeL, 
  shapeO,
  shapeS,
  shapeT,
  shapeZ,
};

class Box {
  Map map;
  
  bool fit(const Position &);
public:
  Box() {
    for (auto & i : map) {
      i.fill(0);
    }
  }

  void draw_map();
  bool check_line_full(int lino);
  void erase_line(int lino);
  int check_erase();
  bool fit(array<int, 6> &, const Position&);
};

class Block {
  Position pos{23,6};
  Shape shape{shapeI};
  int rotate{0};
  Box & box;
public:
  Block(Box& _box, Shape _shape = shapeI) : box(_box), shape(_shape) {}
  array<int, 6> pre_rotate_state();
  bool can_rotate();
  void do_rotate();
};

class Game {
  Box box;
  Block block{box};
  int score = 0;
  int level = 1;
  int time_count = 0;
  static int interval;

public:
  Game() = default;

  void draw_map() {}
  void time_pass(int i);
  int check_erase();
  void erase();
  void run();
  void try_rotate();
  void try_move_right();
  void try_move_left();
  void try_fall();
};

int Game::interval = 5;

#endif