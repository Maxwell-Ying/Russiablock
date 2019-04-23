#include <iostream>
#include <random>

#include "box.hpp"

using std::cout;
using std::endl;

// func for Box
// print func, to see borad and items.
void Box::print(Map &m) {
  cout << "+------------+" << endl;
  for (const auto & i : m) {
    cout << "|";
    for (const auto & j : i) {
      if (j) {
        cout << "x";
      } else {
        cout << " ";
      }
    }
    cout << "|";
    cout << endl;
  }
  cout << "+------------+" << endl;
}

// draw box and the data fixed in box
// currently not use in this project.
void Box::draw_map() {
  reset_pos();
  print(map);
}

// draw map and block in map, 
void Box::draw_map(Position pos, array<int, 6> points) {
  Map tmp = map;
  tmp[pos.first][pos.second] = 1;
  tmp[pos.first+points[0]][pos.second+points[1]] = 1;
  tmp[pos.first+points[2]][pos.second+points[3]] = 1;
  tmp[pos.first+points[4]][pos.second+points[5]] = 1;
  reset_pos();
  print(tmp);
}

bool Box::check_line_full(int lino) {
  auto & line = map[lino];
  for (const auto i : line) {
    if (!i) {
      return false;
    }
  }
  return true;
}

// erase a full line, assume check_line_full is already returned true
void Box::erase_line(int lino) {
  if (lino < 0 || lino >= 23) {
    return ;
  }
  for (; lino>0; lino--) {
    map[lino] = map[lino-1];
  }
  // fill top with 0 is needed
  map[0].fill(0);
}

// check if there is full lines
// there can be multi line full so an array is needed.
array<int, 23> Box::check_erase() {
  array<int, 23> ret;
  ret.fill(0);
  for (auto i=0; i<23; i++) {
    if (check_line_full(i)) {
      ret[i] = 1;
    }
  }
  return ret;
}

// check if a position in box is empty.
bool Box::fit(const Position & p) {
  if (p.first >= 0 && p.first < map.size() && p.second >= 0 && p.second < map[0].size()) {
    return map[p.first][p.second] == 0;
  }
  if (p.first == -1 && p.second >= 0 && p.second < map[0].size()) {
    return true;
  }
  return false;
}

// check if a shape in pos can be place to box
bool Box::fit(array<int, 6> s, Position pos) {
  Position pos1{pos.first, pos.second};
  Position pos2{pos.first + s[0], pos.second + s[1]};
  Position pos3{pos.first + s[2], pos.second + s[3]};
  Position pos4{pos.first + s[4], pos.second + s[5]};
  return fit(pos1) && fit(pos2) && fit(pos3) && fit(pos4);
}

// set a position occupied, used in merge origin items and new block
void Box::set_bit(Position p) {
  map[p.first][p.second] = 1;
} 


// func for block
// if before == true, return current state, else return rotated state
array<int, 6> Block::pre_rotate_state(bool before = false) {
  // rotate time by different shape
  // do 2 or 3 times rotate can result in 4 different shape of I,S,Z
  // and that is not what I want
  int r_time = before ? rotate: rotate + 1;
  if (shape == shapeT || shape == shapeL || shape == shapeJ) {
    // only TLJ have exactly 4 shape
    r_time %= 4;
  } else if (shape == shapeO) {
    // O have only one shape
    r_time = 0;
  } else {
    r_time %= 2;
  }

  array<int, 6> pre = {bitmap[shape]};
  // flip along y=-x then x=0 rotate the shape by 90 degree.
  for (auto i=0; i<r_time; i++) {
    pre = array<int, 6>{pre[1]*-1, pre[0], pre[3]*-1, pre[2], pre[5]*-1, pre[4]};
  }
  return pre;
}

// check if block can rotate in this pos
bool Block::can_rotate() {
  return box.fit(pre_rotate_state(false), pos);
}

// do rotate, simply add rotate by 1
void Block::do_rotate() {
  rotate += 1;
  if (shape == shapeT) {
    rotate %= 4;
  } else if (shape == shapeO) {
    rotate = 0;
  } else {
    rotate %= 2;
  }
}

// check movement
bool Block::can_move_left() {
  return box.fit(pre_rotate_state(true), Position{pos.first, pos.second-1});
}

// do movement
void Block::do_move_left() {
  pos.second -= 1;
}

bool Block::can_move_right() {
  return box.fit(pre_rotate_state(true), Position{pos.first, pos.second+1});
}

void Block::do_move_right() {
  pos.second += 1;
}

bool Block::can_fall() {
  return box.fit(pre_rotate_state(true), Position{pos.first + 1, pos.second});
}

void Block::do_fall() {
  pos.first += 1;
}

// reset pos to top for new block
void Block::reset_pos() {
  pos.first = 0;
  pos.second = 6;
}

void Block::set_shape(Shape s) {
  shape = s;
}

// to fix block to box, after this the block need to be reset
void Block::merge() {
  auto bits = pre_rotate_state(true);
  box.set_bit(pos);
  box.set_bit({pos.first+bits[0], pos.second+bits[1]});
  box.set_bit({pos.first+bits[2], pos.second+bits[3]});
  box.set_bit({pos.first+bits[4], pos.second+bits[5]});
}

void Block::draw_block_and_map() {
  box.draw_map(pos, pre_rotate_state(true));
}

// func for Game
// draw func, there should also be some thing like next block
// score and comic character. this will be in following update.
void Game::draw_map() {
  block.draw_block_and_map();
  cout << debug_info << endl;
}

// for auto fall. if count > count_max then do fall.
void Game::time_pass(int i) {
  time_count += interval;
  if (time_count >= time_count_max) {
    try_fall();
    time_count = 0;
  }
}

// main loop for game. catch keyboard input and react.
void Game::run() {
  init_cursor();
  draw_map();
  while (1) {
    // this draw seem to be too frequent and useless.
    // draw every movement or erase instead.
    // draw_map();
    char ch;
    if (_kbhit()) {
      ch = _getch();
      switch (ch)
      {
      case 'w':
        debug_info = "try_rotate";
        try_rotate();
        break;
      case 'a':
        debug_info = "try_left";
        try_move_left();
        break;
      case 'd':
        debug_info = "try_right";
        try_move_right();
        break;
      case 's':
        debug_info = "try_fall";
        try_fall();
        time_count = 0;
        break;
      default:
        break;
      }
    }
    Sleep(interval);
    time_pass(interval);
  }
}

array<int, 23> Game::check_erase() {
  return box.check_erase();
}

// erase func. remember the array returned is current line and 
// when one line is deleted, all lines above moved down.
// so erase_count is need or any other way to do so.
void Game::erase() {
  array<int, 23> erase = check_erase();
  int erase_count = 0;
  for (int i=22; i>=0; i--) {
    if (erase[i]) {
      box.erase_line(i+erase_count);
      erase_count += 1;
    }
  }
  draw_map();
}

// try func do not exactly do operation.
// only do if can do.
// draw map whether or not really do.
void Game::try_rotate() {
  if (block.can_rotate()) {
    block.do_rotate();
  }
  draw_map();
}

void Game::try_move_left() {
  if (block.can_move_left()) {
    block.do_move_left();
  }
  draw_map();
}

void Game::try_move_right() {
  if (block.can_move_right()) {
    block.do_move_right();
  }
  draw_map();
}

void Game::try_fall() {
  if (block.can_fall()) {
    block.do_fall();
  } else {
    // these is all code for  new block.
    block.merge();
    block.reset_pos();
    block.set_shape(next_shape);
    next_shape = get_next();
    erase();
  }
  draw_map();
}

// get next shape. remember to make the two item static 
// to get "really" random shape.
Shape Game::get_next() {
  static std::uniform_int_distribution<unsigned> u(0, 6);
  static std::default_random_engine e;
  unsigned int tmp = u(e);
  return static_cast<Shape>(tmp);
}

int main(int argc, char * argv[]) {
  Game game;
  game.run();
  return 0;
}