#include "benchmark.h"

#include <mpi.h>

namespace {

constexpr auto kTimescale = 1e3;

template <typename Timer>
Timer Started() {
  Timer timer;
  timer.Start();
  return timer;
}

}  // namespace

namespace benchmark {

OnceTimer OnceTimer::Started() noexcept {
  return ::Started<OnceTimer>();
}

void OnceTimer::Start() {
  start_ = MPI_Wtime();
}

void OnceTimer::Stop() {
  const auto stop = MPI_Wtime();
  total_ = stop - start_;
}

double OnceTimer::Get() const noexcept {
  return total_ * kTimescale;
}

RepeatingTimer RepeatingTimer::Started() noexcept {
  return ::Started<RepeatingTimer>();
}

void RepeatingTimer::Stop() {
  const auto stop = MPI_Wtime();
  total_ += stop - start_;
}

TimerGuard::TimerGuard(OnceTimer& timer) : timer_{timer} {
  timer_.Start();
}

TimerGuard::~TimerGuard() {
  timer_.Stop();
}

}  // namespace benchmark
