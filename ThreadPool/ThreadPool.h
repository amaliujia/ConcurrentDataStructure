//
// Created by 王 瑞 on 16-3-15.
//

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <vector>
#include <thread>
#include <iostream>

class Task{
public:
    Task(int id): id_(id) {}

    void Execute() {
      std::cout << id_ << std::endl;
    }
private:
    int id_;
};


template <typename T>
class TaskQueue {
public:
    T GetTask() {
      std::unique_lock<std::mutex> locker(lock_);
      while (storage_.size() == 0) {
        cond_.wait(locker);
      }

      T item = storage_.front();
      storage_.erase(storage_.begin());
      locker.unlock();
      return item;
    }

    void PutTask(const T& item) {
      std::unique_lock<std::mutex> locker(lock_);
      storage_.push_back(item);
      locker.unlock();
      cond_.notify_one();
    }

    void PutTask(const std::vector<T>& items) {
     std::unique_lock<std::mutex> locker(lock_);
     storage_.push_back(items);
     locker.unlock();
     cond_.notify_all();
    }

private:
    std::vector<T> storage_;
    std::mutex lock_;
    std::condition_variable cond_;
};


class ThreadPool {
public:
    ThreadPool(int num): num_(num), flag(true){
      workers_.resize(num);
    }

    void PutTask(const Task& t) {
      queue_.PutTask(t);
    }

    void Start() {
      for (int i = 0; i < num_; i++) {
        workers_[i] = std::thread(&ThreadPool::WorkerMain, this, &queue_, &flag);
      }
    }

    void Destroy() {
      flag = false;
      for (int i = 0; i < num_; i++) {
        workers_[i].join();
      }
    }
private:
    void WorkerMain(TaskQueue<Task> *queue, bool *flag) {
      while (*flag) {
        Task t = queue->GetTask();
        t.Execute();
      }
    }
private:
    TaskQueue<Task> queue_;
    std::vector<std::thread> workers_;
    int num_;
    bool flag;
};


#endif //THREADPOOL_THREADPOOL_H
