#ifndef OS_THREAD_H_
#define OS_THREAD_H_

#include <thread>
#include <functional>

#include "unit.h"

namespace sarab {

namespace thread {

using void_f_id = void (*)(std::thread::id);

class guardedThread {
public:
  guardedThread(std::function<void(void)> func) : m_t{func} {
  }
  ~guardedThread() {
    if(m_t.joinable()) {
      m_t.join();
    }
  }

  std::thread::id getThreadId() const { return m_t.get_id(); }

  guardedThread(guardedThread const&)             = delete;
  guardedThread& operator=(guardedThread const&)  = delete;

private:
  std::thread m_t;
};

}

}

#endif
