#include <iostream>

#include "box.hpp"

using std::cout;
using std::endl;

// func for Box
void Box::draw_map() {
  reset_pos();
  cout << "+------------+" << endl;
  for (const auto & i : map) {
    cout << "|";
    for (const auto & j : i) {
      if (j) {
        cout << "x";
      } else {
        cout << "o";
      }
    }
    cout << "|";
    cout << endl;
  }
  cout << "+------------" << endl;
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

void Box::erase_line(int lino) {
  if (lino < 0 || lino >= 12) {
    return ;
  }
  for (; lino<11; lino++) {
    map[lino] = map[lino+1];
  }
  map[11].fill(0);
}

int Box::check_erase() {
  for (auto i=0; i<12; i++) {
    if (check_line_full(i)) {
      return i;
    }
  }
  return -1;
}

bool Box::fit(const Position & p) {
  if (p.first >= 0 && p.first < map.size() && p.second >= 0 && p.second < map[0].size) {
    return map[p.first][p.second] == 0;
  }
  if (p.first == -1 && p.second >= 0 && p.second < map[0].size()) {
    return true;
  }
  return false;
}

bool Box::fit(array<int, 6> &s, const Position& pos) {
  Position pos1{pos.first, pos.second};
  Position pos2{pos.first + s[0], pos.second + s[1]};
  Position pos3{pos.first + s[2], pos.second + s[3]};
  Position pos4{pos.first + s[4], pos.second + s[5]};
  return fit(pos1) && fit(pos2) && fit(pos3) && fit(pos4);
}


// func for block
array<int, 6> Block::pre_rotate_state() {
  // rotate time by different shape
  // do 2 or 3 times rotate can result in 4 different shape of I
  // and that is not what I want
  int r_time = rotate + 1;
  if (shape == shapeT) {
    // only T have exactly 4 shape
    r_time %= 4;
  } else if (shape == shapeO) {
    // O have only one shape
    r_time = 0;
  } else {
    r_time %= 2;
  }

  array<int, 6> pre = {bitmap[shape]};
  for (auto i=0; i<r_time; i++) {
    pre = array<int, 6>()
  }
}

bool Block::can_rotate() {
  return box.fit(rotated())
}

// func for Game
void Game::run() {
  while (1) {
    char ch = kbhit();
    if (ch) {
      switch (ch)
      {
      case 'w':
        try_rotate();
        break;
      case 'a':
        try_move_left();
        break;
      case 'd':
        try_move_right();
        break;
      case 's':
        try_fall();
        time_count = 0;
        break;
      default:
        break;
      }
    }
    time_pass(interval);
  }
}

int Game::check_erase() {
  return box.check_erase();
}

void Game::erase() {
  int erase = check_erase();
  if (erase >=0 && erase < 12) {
    box.erase_line(erase);
  }
  draw_map();
}

int main(int argc, char * argv[]) {
  Game game;
  game.run();
  return 0;
}