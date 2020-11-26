#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace DeltaEngine
{

template <typename T>
class Queue
{
  std::queue<T> queue;
  mutable std::mutex m;
  std::condition_variable c;

  public:

  // Race condition
  bool Empty()
  {
    return queue.empty();
  }

  void Enqueue(T data)
  {
    std::lock_guard<std::mutex> lock(m);
    queue.push(data);
    c.notify_one();
  }

  T Dequeue()
  {
    std::unique_lock<std::mutex> lock(m);
    while (queue.empty())
      c.wait(lock);

    T data = queue.front();
    queue.pop();
    return data;
  }
}

}