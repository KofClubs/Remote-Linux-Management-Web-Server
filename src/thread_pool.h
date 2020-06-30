#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
public:
  typedef std::function<void(void)> TASK; /* 任务类型：函数，支持参数表转换 */
  ThreadPool(int); /* 构造函数，参数是线程池容量 */
  ~ThreadPool();   /* 析构函数 */
private:
  int size, capacity;                         /* 空闲线程数目，容量 */
  bool runningStatus;                         /* 线程池运行状态 */
  std::mutex mutexLock;                       /* 互斥锁 */
  std::condition_variable_any conditionEmpty; /* 任务队列空的条件变量 */
  void work(void); /* 工作函数，执行任务队列的任务 */
  std::vector<std::shared_ptr<std::thread>> threadVec; /* 线程对象向量 */
  std::list<TASK> taskList;                            /* 任务队列 */
public:
  void turnOn();      /* 启动线程池 */
  void turnOff();     /* 关闭线程池 */
  void addTask(TASK); /* 向任务队列添加任务 */
};

#endif