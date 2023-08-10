#pragma once
#include <functional>
class Thread
{
public:
    using ThreadFunc = std::function<void()>;
    Thread(ThreadFunc func);
    ~Thread();
    void start();

private:
    ThreadFunc func_;
};
