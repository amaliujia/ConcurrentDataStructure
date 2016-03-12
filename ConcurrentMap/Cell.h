//
// Created by 王 瑞 on 16-3-12.
//

#ifndef CONCURRENTMAP_CELL_H
#define CONCURRENTMAP_CELL_H


#include <atomic>

template <typename Key, typename Value>
class Cell {


private:
    std::atomic<Key> key;
    std::atomic<Value> value;
};


#endif //CONCURRENTMAP_CELL_H
