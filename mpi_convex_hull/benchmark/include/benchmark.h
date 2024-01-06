#pragma once

namespace benchmark {

// NOLINTNEXTLINE(hicpp-special-member-functions): False-positive.
class OnceTimer {
 public:
  [[nodiscard]] static OnceTimer Started() noexcept;
  virtual ~OnceTimer() = default;

  void Start();
  virtual void Stop();

  [[nodiscard]] double Get() const noexcept;

 protected:
  double start_{};
  double total_{};
};

class RepeatingTimer final : public OnceTimer {
 public:
  [[nodiscard]] static RepeatingTimer Started() noexcept;

  void Stop() override;
};

class TimerGuard {
 public:
  explicit TimerGuard(OnceTimer&);
  ~TimerGuard();

  TimerGuard(const TimerGuard&) = delete;
  TimerGuard& operator=(const TimerGuard&) = delete;
  TimerGuard(TimerGuard&&) = delete;
  TimerGuard& operator=(TimerGuard&&) = delete;

 private:
  OnceTimer& timer_;
};

}  // namespace benchmark
