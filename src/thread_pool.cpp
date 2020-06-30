#include "thread_pool.h"

#include <cstddef>
#include <cstdio>

ThreadPool::ThreadPool(int n)
    : size(0), capacity(n), runningStatus(false), threadVec(capacity) {}

ThreadPool::~ThreadPool() {
  if (runningStatus)
    turnOff();
}

void ThreadPool::work(void) {
  TASK t = NULL;
  while (runningStatus) {
    std::lock_guard<std::mutex> guard(mutexLock);
    // 任务队列空，等待任务唤醒
    if (taskList.empty())
      conditionEmpty.wait(mutexLock);
    // 获取任务
    if (!taskList.empty()) {
      t = taskList.front(); /* 取走任务 */
      taskList.pop_front(); /* 删除任务 */
    } else
      continue;
    t(); /* 执行任务 */
  }
}

void ThreadPool::turnOn() {
  for (int i = 0; i < capacity; i++) {
    threadVec.push_back(std::make_shared<std::thread>(
        std::bind(&ThreadPool::work, this))); /* 创建线程 */
    size++;
  }
  runningStatus = true;
  printf("%s\n", "Thread pool ON...");
}

void ThreadPool::turnOff() {
  for (auto t : threadVec)
    t->join(); /* 等待线程终止 */
  runningStatus = false;
  printf("%s\n", "Thread pool OFF!");
}

void ThreadPool::addTask(TASK t) {
  std::lock_guard<std::mutex> guard(mutexLock);
  taskList.push_front(t);
  conditionEmpty.notify_one(); /* 唤醒某线程来执行任务 */
}