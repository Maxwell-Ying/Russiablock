#ifndef _BOX_H
#define _BOX_H

#include <windows.h>
#include <conio.h>
#include <array>
#include <string>

using std::array;
using std::pair;
using std::string;

array<array<int, 6>, 7> bitmap{
  {-1,  0, 1, 0, 2,  0,  // I
   -1,  0, 1, 0, 1, -1, // J
   -1,  0, 1, 0, 1,  1,  // L
    0,  1, 1, 0, 1,  1,   // O
    0,  1, 1, 0, 1, -1,  // S
    1,  0, 0, 1, 0, -1,  // T
    0, -1, 1, 0, 1,  1,}  // Z
};

typedef array<array<int, 12>, 23> Map;
typedef pair<int, int> Position;

// set cursor to needed place.
void set_pos(short x, short y) {
  COORD pos{x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}

// make cursor invisible. set size is needed or this doesnot work.
void init_cursor() {
  _CONSOLE_CURSOR_INFO ccinfo;
  ccinfo.bVisible = false;
  ccinfo.dwSize = 1;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ccinfo);
}

// set pos to left up for new draw.
void reset_pos() {
  set_pos(0, 0);
}

// shape. order is respective to bitmap
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
  // board
  Map map;
  
  bool fit(const Position &);
public:
  Box() {
    for (auto & i : map) {
      i.fill(0);
    }
  }

  void draw_map();
  void draw_map(Position, array<int, 6>);
  bool check_line_full(int lino);
  void erase_line(int lino);
  array<int, 23> check_erase();
  bool fit(array<int, 6> , Position);
  void set_bit(Position);
  void print(Map &m);
};

class Block {
  // center of block. with 3 points refered by shape and bitmap
  Position pos{0,6};
  Shape shape;
  // rotate state of shape, only really rotate when check or draw.
  int rotate{0};
  // a block need a box to validate movement.
  Box & box;
public:
  Block(Box& _box, Shape _shape = shapeT) : box(_box), shape(_shape) {}
  array<int, 6> pre_rotate_state(bool before);
  bool can_rotate();
  void do_rotate();
  bool can_move_left();
  void do_move_left();
  bool can_move_right();
  void do_move_right();
  bool can_fall();
  void do_fall();
  void merge();
  void reset_pos();
  void set_shape(Shape);
  void draw_block_and_map();
};

class Game {
  Box box;
  Block block{box};
  // next shape of block, can also place to block class
  Shape next_shape{shapeT};
  string debug_info;
  // score
  int score = 0;
  int level = 1;
  int time_count = 0;
  int time_count_max = 1000;
  // interval between two kb check. decrease to get high presion
  // increse to get better performance.
  static int interval;

public:
  Game() = default;

  void draw_map();
  void time_pass(int i);
  array<int, 23> check_erase();
  void erase();
  void run();
  void try_rotate();
  void try_move_right();
  void try_move_left();
  void try_fall();
  Shape get_next();
};

// init for static item.
int Game::interval = 5;

#endif