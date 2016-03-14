#include <iostream>
#include <assert.h>
#include <thread>
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


void NormalTest(TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> *mgr) {
  mgr->Set(1, 1);
  mgr->Set(1, 2);
  mgr->Set(3, 3);

  assert(mgr->Get(1) == 2);
  assert(mgr->Get(3) == 3);
}

void Insert(TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> *mgr, int start, int count, int id) {
  for (int i = start; i < (start + count); i++) {
    mgr->Set(i, id);
  }
}

void ThreadedOverlappingTest(TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> *mgr) {
  int a = 1, b = 2, c = 3;
  int count = 10;
  int id1 = 1, id2 = 2, id3 = 3;
  std::thread thread_one(Insert, mgr, a, count, id1);
  std::thread thread_two(Insert, mgr, b, count, id2);
  std::thread thread_three(Insert, mgr, c, count, id3);

  thread_one.join();
  thread_two.join();
  thread_three.join();
  mgr->Print();
}

void ThreadedTest(TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> *mgr) {
  int a = 1, b = 11, c = 21;
  int count = 10;
  std::thread thread_one(Insert, mgr, a, count, 1);
  std::thread thread_two(Insert, mgr, b, count, 2);
  std::thread thread_three(Insert, mgr, c, count, 3);

  thread_one.join();
  thread_two.join();
  thread_three.join();
  mgr->Print();
}


int main() {
  IntHasher hasher;
  IntEqualChecker equalChecker;
  TableManager<int, int, IntHasher, IntEqualChecker, IntEqualChecker> table(hasher, equalChecker, equalChecker, 0, 100);


//  NormalTest(&table);
//  ThreadedTest(&table);
//  ThreadedOverlappingTest(&table);

  return 0;
}