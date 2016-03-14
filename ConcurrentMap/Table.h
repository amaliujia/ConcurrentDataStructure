//
// Created by 王 瑞 on 16-3-12.
//

#ifndef CONCURRENTMAP_TABLE_H
#define CONCURRENTMAP_TABLE_H

#include <vector>
#include <atomic>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

template <typename Key, typename Value>
struct Cell{
  Key key;
  Value value;
};

template <typename Key, typename Value, class KeyHasher, class KeyEqualChecker, class ValueEqualChecker>
class Table {
public:


    Table(KeyHasher hasher, KeyEqualChecker checker, ValueEqualChecker vchecker, Value v, int capacity):
      default_value_(v), keyEqualChecker_(checker), valueEqualChecker_(vchecker), hasher_(hasher), capacity_(capacity) {
      Init();
    }

    ~Table() {

    }
private:
    void Init() {
      cells.resize(capacity_);

      for (size_t i = 0; i < capacity_; i++) {
        this->cells[i] = nullptr;
      }

      next_table_ = nullptr;
    }

    bool SearchSet(Cell<Key, Value> *cell_ptr, Key k, Value v) {

    }

    inline Cell<Key, Value> * CreateCell(Key k, Value v) {
      Cell<Key, Value> *ret_ptr = new Cell<Key, Value>();
      ret_ptr->key = k;
      ret_ptr->value = v;
      return ret_ptr;
    };

    bool IncrementUsed() {
      int cur_used = used_;
      return __sync_bool_compare_and_swap(&used_, cur_used, cur_used + 1);
    }

    bool DecrementUsed() {
      int cur_used = used_;
      return __sync_bool_compare_and_swap(&used_, cur_used, cur_used - 1);
    }

public:
    bool Set(Key k, Value v){
      assert(!valueEqualChecker_(v, default_value_));

      size_t hash_code = hasher_(k);

      while(true) {
        Cell<Key, Value> *cell_ptr = this->cells[hash_code % capacity_];
        // search and set
        if (SearchSet(cell_ptr, k, v)) {
          return true;
        }

        // Key not exist
        // 1. check used_
        // 2. if overflow, start migration
        // 3. if not overflow, update used_.
        // 4. If fail update used_, check overflow. If overflow, start migration
        // 5. If succeed, do insert.
        if (used_ > capacity_ * load_factor_ || used_ >= capacity_) {
          return false;
        } else {
          auto new_cell_ptr = CreateCell(k, v);
          if (__sync_bool_compare_and_swap(&this->cells[hash_code % capacity_]), cell_ptr, new_cell_ptr) {
            // update used_
            IncrementUsed();
            // check used_

          } else {

          }
        }
      }

      return false;
    }

    Value Get(Key k) {
      size_t hash_code = hasher_(k);


      return default_value_;
    }

    void Print() {

    }

private:
    const int capacity_;
    const float load_factor_ = 0.75;
    Value default_value_;

    KeyHasher hasher_;
    KeyEqualChecker keyEqualChecker_;
    ValueEqualChecker valueEqualChecker_;

    size_t used_;
    std::vector<Cell<Key , Value> *> cells;

    Table<Key, Value, KeyHasher, KeyEqualChecker, ValueEqualChecker> *next_table_;
};

template <typename Key, typename Value, class KeyHasher, class KeyEqualChecker, class ValueEqualChecker>
class TableManager {
public:
    TableManager(KeyHasher hasher, KeyEqualChecker checker, ValueEqualChecker vchecker, Value default_value, int capacity) {
      this->table_ = new Table<Key, Value, KeyHasher, KeyEqualChecker, ValueEqualChecker>(hasher, checker, vchecker, default_value, capacity);
    }

    ~TableManager() {
      delete table_;
    }

    void Set(Key k, Value v) {
      while (!this->table_->Set(k, v)) {
        // should immigration
        std::cout << "Wired" << std::endl;
      }
    }

    Value Get(Key k) {
      return this->table_->Get(k);
    }


    void Print() {
      this->table_->Print();
    }

private:
    // Two things need to mention
    // 1. Multiversion controll (one key, more than one version. Must retry for the non-first players)
    // 2. Make sure Migration finish. (Compare counts of Migration flag to used_)
    void Migrate();

private:
    Table<Key, Value, KeyHasher, KeyEqualChecker, ValueEqualChecker> *table_;
};


#endif //CONCURRENTMAP_TABLE_H
