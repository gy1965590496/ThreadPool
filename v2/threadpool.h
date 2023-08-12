#pragma once
#include "thread.h"
#include "task.h"
#include <iostream>
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>

enum class PoolMode{
    CACHE,
    FIXED,
};


class ThreadPool{
public:
    ThreadPool();
    ~ThreadPool();
    void setMode(PoolMode mode);
    void setInitThreshHold(int threshhold);
    void setTaskQueMaxThreshHold(int threshhold);
    void submitTask(std::shared_ptr<Task> sp);
    void start(int initThreadSize = 4);
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    void threadFunc();

private:
    // 
    // std::vector<Thread*> threads_;  // 无法自动释放
    std::vector<std::unique_ptr<Thread>> threads_;
    std::queue<std::shared_ptr<Task>> taskQue_;
    // 
    int initThreadSize_;
    int taskQueMaxThreshHold_;
    std::atomic_int taskSize_;
    PoolMode poolMode_;
    //
    std::mutex taskQueMtx_;
    std::condition_variable notFull_;
    std::condition_variable notEmpty_;   

};