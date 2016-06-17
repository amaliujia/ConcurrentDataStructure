//
// Created by 王 瑞 on 16-6-16.
//

#include "ConcurrentStack.h"

#include <iostream>

using namespace std;

int main() {
  ConcurrentDataStructure::ConcurrentStack<int> stack;
  stack.Push(0);
//  stack.pop();

  cout << stack.size() << endl;

  return 0;
}