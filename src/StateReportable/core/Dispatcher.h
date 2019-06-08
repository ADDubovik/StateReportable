#pragma once

#include <memory>
#include <atomic>
#include <future>
#include <thread>
#include <mutex>
#include <type_traits>


namespace StateReportable::core
{
  // Forward declaration
  template<typename Destination_t>
  class DispatcherTestClass;

  // Modified weak_ptr-based singleton - to allow safe calls from destructors.
  // Thread-safe.
  // Creates just one Destination_t instance, and use Destination_t::send function
  // to redirect data from single thread.
  // Destination_t requirements:
  // - default-constructible;
  // - should provide Destination_t::Data move-constructible type;
  // - should provide void send(Destination_t::Data &&) function.
  template<typename Destination_t>
  class Dispatcher
  {
  public:
    using Data = typename Destination_t::Data;
    using Destination = Destination_t;

    static_assert(std::is_default_constructible_v<Destination_t>);
    static_assert(std::is_move_constructible_v<Data>);

    friend class DispatcherTestClass<Destination>;

    ~Dispatcher() = default;

    Dispatcher(const Dispatcher &) = delete;
    Dispatcher(Dispatcher &&) = delete;
    Dispatcher &operator=(const Dispatcher &) = delete;
    Dispatcher &operator=(Dispatcher &&) = delete;

    static std::weak_ptr<Dispatcher> instanceWeak();

    void send(Data &&data);

  private:
    Dispatcher();

    using My_t = Dispatcher<Destination>;

    using Storage = std::vector<Data>;
    using Exchanger = std::pair<std::mutex, Storage>;

    void dispatch(Exchanger &exchanger, std::atomic<bool> &stop, Destination &destination);

    // Order of member variables is important!
    Exchanger m_exchanger;
    std::atomic<bool> m_stop = false;
    // Should be used in dispatch function only, and in tests via DispatcherTest
    Destination m_destination;
    std::future<void> m_thread;
    // to set m_stop to true
    std::unique_ptr<void, std::function<void (void *)>> m_scopeGuard;
  };
} // namespace StateReportable::core
