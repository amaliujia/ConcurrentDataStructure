#include <iostream>
#include <random>
#include "ThreadPool.h"

using namespace std;

int main() {
  std::mt19937 generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
  ThreadPool pool(4);
  pool.Start();

  for (int i = 0; i < 10000; i++) {
    Task t(i);
    pool.PutTask(t);
  }

  //std::this_thread::sleep_for(6);
  pool.Destroy();

  return 0;
}