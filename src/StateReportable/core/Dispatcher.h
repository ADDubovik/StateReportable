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
    Dispatcher()
        : m_thread(std::async(std::launch::async,
                              &My_t::dispatch,
                              this,
                              std::ref(m_exchanger),
                              std::ref(m_stop),
                              std::ref(m_destination)
        )),
        m_scopeGuard{this, [this](void *) { m_stop = true; }}
    {};

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


  template<typename Destination_t>
  void Dispatcher<Destination_t>::send(Data &&data)
  {
    std::lock_guard<std::mutex> guard(m_exchanger.first);
    if ( m_exchanger.second.size() == m_exchanger.second.capacity() )
      m_exchanger.second.reserve(std::max<size_t>(1u, m_exchanger.second.size() * 2));

    m_exchanger.second.emplace_back(std::move(data));
  }


  template<typename Destination_t>
  void Dispatcher<Destination_t>::dispatch(Exchanger &exchanger, std::atomic<bool> &stop, Destination &destination)
  {
    std::vector<Data> localStorage;

    auto grabExchangerToLocal = [&localStorage](auto &exch)
    {
      std::lock_guard<std::mutex> guard(exch.first);
      for ( auto &&elem : exch.second )
      {
        if ( localStorage.size() == localStorage.capacity() )
          localStorage.reserve(std::max<size_t>(1u, localStorage.size() * 2));

        localStorage.emplace_back(std::move(elem));
      }
      exch.second.clear();
    };

    auto sendLocalToDestination = [&localStorage, &destination]()
    {
      for ( auto &&elem : localStorage )
        destination.send(std::move(elem));
      localStorage.clear();
    };

    while ( !stop )
    {
      grabExchangerToLocal(exchanger);
      sendLocalToDestination();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // ... and once more after stop
    grabExchangerToLocal(exchanger);
    sendLocalToDestination();
  }
} // namespace StateReportable::core
