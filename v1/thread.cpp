#include "thread.h"
#include <iostream>
#include <thread>
#include <functional>

// void threadPrint(){
//     std::cout<<"thread runing!"<<std::endl;
// }

Thread::Thread(ThreadFunc func):func_(func){

}

Thread::~Thread(){

}

void Thread::start(){
    std::thread t(func_);
    t.detach();
}