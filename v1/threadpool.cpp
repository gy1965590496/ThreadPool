#include "threadpool.h"
#include <iostream>
#include <thread>

const int TASK_MAX_THRESHHOLD = 1024;

ThreadPool::ThreadPool()
    :initThreadSize_(4)
    ,taskQueMaxThreshHold_(TASK_MAX_THRESHHOLD)
    ,taskSize_(0)
    ,poolMode_(PoolMode::FIXED)
{

}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::start(int initThreadSize){
    setInitThreshHold(initThreadSize_);
    for(int i=0;i<initThreadSize_;i++){
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::threadFunc, this)));
    }
    for(int i=0;i<initThreadSize_;i++){
        threads_[i]->start();
    }
}

void ThreadPool::threadFunc(){
    std::cout<<"begin threadFunc"<<std::endl;
    std::cout<<std::this_thread::get_id()<<std::endl;
    std::cout<<"end threadFunc"<<std::endl;
}


void ThreadPool::setInitThreshHold(int threshhold){
    initThreadSize_ = threshhold;
}

void ThreadPool::setMode(PoolMode mode){
    poolMode_ = mode;
}

void ThreadPool::setTaskQueMaxThreshHold(int threshhold){
    taskQueMaxThreshHold_ = threshhold;
}