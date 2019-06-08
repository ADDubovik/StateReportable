#pragma once

#include <memory>
#include <atomic>
#include <future>
#include <thread>
#include <mutex>
#include <type_traits>


namespace StateReportable
{
  // Modified weak_ptr-based singleton - to allow safe calls from destructors.
  // Thread-safe.
  // Data is redirecting from single thread to Destination_t::send.
  template<typename Data_t, typename Destination_t>
  class Dispatcher
  {
    static_assert(std::is_default_constructible_v<Destination_t>);
    static_assert(std::is_move_constructible_v<Data_t>);

  public:
    using Data = Data_t;
    using Destination = Destination_t;

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
                              std::ref(m_stop)
        )),
        m_scopeGuard{this, [this](void *) { m_stop = true; }}
    {};

    using My_t = Dispatcher<Data, Destination>;

    using Storage = std::vector<Data>;
    using Exchanger = std::pair<std::mutex, Storage>;

    void dispatch(Exchanger &exchanger, std::atomic<bool> &stop);

    // Order is important!
    Exchanger m_exchanger;
    std::atomic<bool> m_stop = false;
    std::future<void> m_thread;
    // to set m_stop to true
    std::unique_ptr<void, std::function<void (void *)>> m_scopeGuard;
  };


  template<typename Data_t, typename Destination_t>
  std::weak_ptr<Dispatcher<Data_t, Destination_t>> Dispatcher<Data_t, Destination_t>::instanceWeak()
  {
    // new needed and no std::make_shared because of private c-tor
    static auto inst = std::shared_ptr<My_t>(new My_t());
    return inst;
  }


  template<typename Data_t, typename Destination_t>
  void Dispatcher<Data_t, Destination_t>::send(Data &&data)
  {
    std::lock_guard<std::mutex> guard(m_exchanger.first);
    if ( m_exchanger.second.size() == m_exchanger.second.capacity() )
      m_exchanger.second.reserve(std::max(static_cast<size_t>(1u), m_exchanger.second.size() * 2));

    m_exchanger.second.emplace_back(std::move(data));
  }


  template<typename Data_t, typename Destination_t>
  void Dispatcher<Data_t, Destination_t>::dispatch(Exchanger &exchanger, std::atomic<bool> &stop)
  {
    std::vector<Data> localStorage;
    Destination destination;

    auto grabExchangerToLocal = [&localStorage](auto &exch)
    {
      std::lock_guard<std::mutex> guard(exch.first);
      for ( auto &&elem : exch.second )
      {
        if ( localStorage.size() == localStorage.capacity() )
          localStorage.reserve(std::max(1u, localStorage.size() * 2));

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
} // namespace StateReportable
