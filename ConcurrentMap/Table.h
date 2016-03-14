//
// Created by 王 瑞 on 16-3-12.
//

#ifndef CONCURRENTMAP_TABLE_H
#define CONCURRENTMAP_TABLE_H


#include <atomic>
#include <stdlib.h>
#include <iostream>

constexpr int capacity = 100;

template <typename Key, typename Value>

struct Cell{
  Key key;
  Value value;
};

template <typename Key, typename Value, class KeyHasher, class KeyEqualChecker, class ValueEqualChecker>
class Table {
public:
    Table(): used_(0) {
    }

    Table(KeyHasher hasher): used_(0), hasher_(hasher) {}

    Table(KeyHasher hasher, KeyEqualChecker checker):
      hasher_(hasher), keyEqualChecker_(checker) {
      Table();
    }

    Table(KeyHasher hasher, KeyEqualChecker checker, Key k):
      default_key_(k) {
      Table(hasher, checker);
    }

    Table(KeyHasher hasher, KeyEqualChecker checker, ValueEqualChecker vchecker, Key k, Value v):
      default_value_(v), IfMigrating(false), valueEqualChecker_(vchecker) {
      Table(hasher, checker, k);
      init();
    }

private:
    void init() {
      for (size_t i = 0; i < capacity; i++) {
        this->cells[i].key = default_key_;
        this->cells[i].value = default_value_;
      }
    }

public:
    bool Set(Key k, Value v){
      size_t hash_code = hasher_(k);

      for (size_t i = hash_code % capacity; ; i++) {
          if (i == capacity) {
            i = 0;
          }
          Key cur_key = this->cells[i].key;

          if (keyEqualChecker_(cur_key, k)) {
            Value cur_val = this->cells[i].value;
            while (true) {
              if (valueEqualChecker_(cur_val, default_value_) && IfMigrating == true) {
                // TODO:go to another table
                return true;
              }

              // value is only default value.
              if(__sync_bool_compare_and_swap(&(this->cells[i].value), cur_val, v)) {
                return true;
              }

              cur_val = this->cells[i].value;
            }
            return true;
          }

          if (keyEqualChecker_(cur_key, default_key_)) {
            size_t cur_used = used_;

            if (cur_used >= (int(capacity * 0.75))) {
              // start migration
              return false;
            }

            while (!__sync_bool_compare_and_swap(&used_, cur_used, cur_used + 1)) {
              cur_used++;
              if (cur_used >= int(capacity * 0.075)) {
                // start migration
                return false;
              }
            }

            // If successfully get one slot:

            if (__sync_bool_compare_and_swap(&(this->cells[i].key), default_key_, k)) {
              Value cur_val = this->cells[i].value;

              while (!__sync_bool_compare_and_swap(&(this->cells[i].value), cur_val, v)) {
                cur_val = this->cells[i].value;
              }
              return true;
            }

          }
      }
    }

    Value Get(Key k) {
      size_t hash_code = hasher_(k);
      size_t count = 0;
      for (size_t i = hash_code % capacity; count < capacity ; i++, count++) {
        if (i == capacity) {
          i = 0;
        }
        Key cur_key = this->cells[i].key;

        if (keyEqualChecker_(cur_key, k)) {
          Value ret_val = this->cells[i].value;

          if (ret_val != default_value_) {
            return ret_val;
          } else {
            return default_value_;
          }
        }
      }
      return default_value_;
    }

    void Print() {
      for (size_t i = 0; i < capacity; i++) {
        if (this->cells[i].key != default_key_) {
          std::cout << this->cells[i].key << " " << this->cells[i].value << std::endl;
        }
      }
    }

private:
    Key default_key_;
    Value default_value_;

    KeyHasher hasher_;
    KeyEqualChecker keyEqualChecker_;
    ValueEqualChecker valueEqualChecker_;

    size_t used_;
    Cell<Key , Value> cells[capacity];

    std::atomic_bool IfMigrating;

};

template <typename Key, typename Value, class KeyHasher, class KeyEqualChecker, class ValueEqualChecker>
class TableManager {
public:
    TableManager(KeyHasher hasher, KeyEqualChecker checker, ValueEqualChecker vchecker, Key default_key, Value default_value) {
      this->table_ = new Table<Key, Value, KeyHasher, KeyEqualChecker, ValueEqualChecker>(hasher, checker, vchecker, default_key, default_value);
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
    Table<Key, Value, KeyHasher, KeyEqualChecker, ValueEqualChecker> *table_;
};


#endif //CONCURRENTMAP_TABLE_H
