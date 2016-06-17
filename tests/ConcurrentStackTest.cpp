//
// Created by 王 瑞 on 16-6-16.
//

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "../ConcurrentStack/ConcurrentStack.h"

class StackTest : public ::testing::Test { };


TEST_F(StackTest, BasicTest) {
  ConcurrentDataStructure::ConcurrentStack<int> stack;
//  stack.Push(0);
//  stack.Push(1);
//  stack.Pop();
//  stack.Pop();

  EXPECT_EQ(stack.size(), 0);
}

TEST_F(StackTest, BasicTest2) {
  ConcurrentDataStructure::ConcurrentStack<int> stack;
  stack.Push(0);

  int a = -1;
  EXPECT_EQ(stack.Top(a), true);
  EXPECT_EQ(a, 0);
  stack.Clear();
}