#pragma once

namespace DeltaEngine
{
  template <typename T>
  class Singleton
  {
  public:
    // In C++11, static variable in function are not created until function is run
    static T& Instance()
    {
      static T instance; // Create T instance on stack
      return instance; // Return static T instance
    }

  protected:
    Singleton() = default;
    ~Singleton() = default;

  private:
    Singleton(const Singleton&) = delete; // delete copy ctor
    Singleton& operator=(const Singleton&) = delete; // delete copy assignment
  };
}
