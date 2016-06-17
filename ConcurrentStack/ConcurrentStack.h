//
// Created by 王 瑞 on 16-6-16.
//

#ifndef CONCURRENTDATASTRUCTURE_CONCURRENTSTACK_H
#define CONCURRENTDATASTRUCTURE_CONCURRENTSTACK_H

#include <atomic>
#include <locale>

namespace ConcurrentDataStructure {


template<typename ValueType>
class ConcurrentStack {
public:

  struct stack_block {
    ValueType value;
    stack_block *next_block;
  };


  ConcurrentStack(): stack_head(nullptr), length(0) {}

  ~ConcurrentStack() {
    Clear();
  }

  size_t size() const {
    return length.load();
  }

  bool Top(ValueType& value) {
    stack_block *phead = nullptr;

    phead = stack_head;

    if (phead == nullptr) {
      return false;
    } else {
      value = phead->value;
      return true;
    }
  }

  void Push(const ValueType& value) {
    stack_block *phead = nullptr;
    stack_block *pnow = new stack_block();

    pnow->value = value;

    while (true) {
      phead = stack_head;
      pnow->next_block = phead;
      if (__sync_bool_compare_and_swap(&stack_head, phead, pnow)) {
        length++;
        break;
      }
    }
  }

  void Pop() {
    stack_block *phead = nullptr;
    stack_block *pnext = nullptr;

    while (true) {
      phead = stack_head;
      pnext = phead->next_block;

      if (__sync_bool_compare_and_swap(&stack_head, phead, pnext)) {
        delete phead;
        length--;
        break;
      }
    }
  }


  void Clear() {
    stack_block *phead = nullptr;
    stack_block *pnext = nullptr;


    while (true) {
      phead = stack_head;

      if (phead == nullptr) {
        return;
      }

      pnext = phead->next_block;

      if (__sync_bool_compare_and_swap(&stack_head, phead, pnext)) {
        delete phead;
        length--;
      }
    }
  }

private:
  stack_block *stack_head;
  std::atomic<size_t> length;
};

}

#endif //CONCURRENTDATASTRUCTURE_CONCURRENTSTACK_H
