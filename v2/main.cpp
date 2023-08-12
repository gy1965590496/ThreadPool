#include <iostream>
#include <chrono>
#include <thread>
#include "threadpool.h"
#include "task.h"
#include <string>
#include <sstream>
#include "semaphore.h"

class MyTask : public Task
{
public:
    void run(){
        std::cout<<"tid:"<<std::this_thread::get_id()<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        // 任务处理。。。
        std::stringstream ss;
        ss << std::this_thread::get_id();
        // 将任务返回值放入结果变量中
        setResult(ss.str());
    }
    void setResult(std::string s) {
        result_ = s;
        sem_.post();
    }
    std::string getResult() {
        sem_.wait();
        return result_;
    }
private:
    std::string result_;
    Semaphore sem_; // 线程通信信号量
};



int main(){
    ThreadPool pool;
    pool.start(4);
    auto task1 = std::make_shared<MyTask>();
    auto task2 = std::make_shared<MyTask>();
    auto task3 = std::make_shared<MyTask>();
    auto task4 = std::make_shared<MyTask>();
    pool.submitTask(task1);
    pool.submitTask(task2);
    pool.submitTask(task3);
    pool.submitTask(task4);
    std::cout<<"task1:"<<task1->getResult()<<std::endl;
    std::cout<<"task2:"<<task2->getResult()<<std::endl;
    std::cout<<"task3:"<<task3->getResult()<<std::endl;
    std::cout<<"task4:"<<task4->getResult()<<std::endl;

    // std::this_thread::sleep_for(std::chrono::seconds(5));
    getchar();
    return 0;
}