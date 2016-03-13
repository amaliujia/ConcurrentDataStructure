#include <iostream>
#include "Table.h"

using namespace std;

struct IntHasher {
    inline size_t operator()(int i) const {
      return std::hash<int>()(i);
    }
};

struct IntEqualChecker {
    inline bool operator()(const int& l, const int& r) const {
      return l == r;
    }
};

int main() {
  IntHasher hasher;
  IntEqualChecker equalChecker;
  TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> table(hasher, equalChecker, equalChecker, 0, 0);
  table.Set(1, 1);


//  cout << cell.SetItem(0, 1.5) << endl;
//  cout << sizeof(table) << endl;
//  table.test();

  return 0;
}