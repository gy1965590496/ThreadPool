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
    // 创建线程对象
    for(int i=0;i<initThreadSize_;i++){
        // 创建线程对象的时候，将线程函数threadFunc传给线程对象
        auto t = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
        threads_.emplace_back(std::move(t));
    }
    // 启动所有线程
    for(int i=0;i<initThreadSize_;i++){        
        threads_[i]->start();
    }
}

void ThreadPool::submitTask(std::shared_ptr<Task> sp){
    // 要访问任务队列，获取锁
    std::unique_lock<std::mutex> lock(taskQueMtx_);

    // 等待队列不满这个条件发送，等待时间最长为1s
    if(!notFull_.wait_for(lock, std::chrono::seconds(1), [&]()->bool { return taskQue_.size() < (size_t)taskQueMaxThreshHold_;})){
        std::cerr<<"taskque is full!"<<std::endl;
        return;
    }
    // 添加任务至任务队列
    taskQue_.emplace(sp);
    taskSize_++;

    // 添加任务后任务队列非空，因此在“不空”条件上进行通知
    notEmpty_.notify_all();
}

void ThreadPool::threadFunc(){
    // std::cout<<"begin threadFunc"<<std::endl;
    // std::cout<<std::this_thread::get_id()<<std::endl;
    // std::cout<<"end threadFunc"<<std::endl;

    

    for(;;){        
        std::shared_ptr<Task> task;
        // 出了作用域自动释放锁
        {
            // 要访问任务队列，获取锁
            std::unique_lock<std::mutex> lock(taskQueMtx_);
            std::cout << "tid:" << std::this_thread::get_id()
				<< "尝试获取任务..." << std::endl;

            // 等待“不空”这个条件
            notEmpty_.wait(lock, [&]()->bool { return taskQue_.size() > 0;});
            // 从队列中取出一个任务，并移除
            task = taskQue_.front();
            taskQue_.pop();
            taskSize_--;

            // 如果队列中仍然有任务，继续通知其他线程消费
            if(taskQue_.size() > 0){
                notEmpty_.notify_all();
            }

            // 因为消费了任务后队列不满了，因此通知生产者继续生产
            notFull_.notify_all();

        }
        // 执行任务
        if(task!=nullptr){
            task->run();
        }
        
    }
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