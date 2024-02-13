//
// Created by Tony Adriansen on 2/9/24.
//

#ifndef RAY_TRACE_SRC_TIMER_HPP_
#define RAY_TRACE_SRC_TIMER_HPP_

#include <chrono>

class Timer {

 public:
  Timer() {
    reset();
  };

  void reset() {
    m_startTime = std::chrono::high_resolution_clock::now();
  }

  float elapsedNanoSeconds() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now() - m_startTime).count();
  }
  float elapsedMicroSeconds() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - m_startTime).count() ;
  }
  float elapsedMilliSeconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - m_startTime).count();
  }
  float elapsedSeconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::high_resolution_clock::now() - m_startTime).count();
  }
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};
#endif //RAY_TRACE_SRC_TIMER_HPP_
