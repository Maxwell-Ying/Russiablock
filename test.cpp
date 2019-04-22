#include <iostream>
#include <array>

using namespace std;

int main() {
  array<array<int, 6>, 7> bitmap{
    {-1, 0, 1, 0, 2, 0,  // I
    -1, 0, 1, 0, 1, -1 // J
    -1, 0, 1, 0, 1, 1,  // L
    0, 1, 1, 0, 1, 1,   // O
    0, 1, 1, 0, 1, -1,  // S
    1, 0, 0, 1, 0, -1,  // T
    0, -1, 1, 0, 1, 1,}  // Z
  };
  for (auto & i : bitmap) {
    for (auto j : i) {
      cout << j << endl;
    }
  }
  return 0;
}