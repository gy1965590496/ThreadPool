# 线程池v1

## 基本草图

![1691657547859](/src/1691657547859.png)



## 类的定义

- ThreadPool：用于管理和启动线程池
- Thread：用于启动任务
- Task：定义任务的接口



## 类的成员变量定义

### ThreadPool

1.任务和线程池

- 线程池列表
- 任务队列（非线程安全的）

2.属性

- 线程池线程数量
- 任务队列最大长度
- 任务数量（非线程安全的）
- 线程池模式

3.线程安全相关

- 任务队列锁
- 任务队列条件变量



### Thread

函数对象

### Task



## 类的成员方法定义

### ThreadPool

- 构造、析构
- 启动线程池
- 设置模式、设置初始线程数量、设置task上限
- 提交任务

### Thread

- 开启线程

### Task

- 任务运行接口



## 问题

task是threadpool的私有变量，thread类想要通过threadpool运行task如何做到。

解决方案OOP：使用绑定器将threadpool的成员方法封装为函数对象传递给thread类，thread就可以以该方法作为线程启动的函数进行运行。
