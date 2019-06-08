#include "Dispatcher.h"
#include "TestDestination.h"


namespace StateReportable::core
{
  template<typename Destination_t>
  Dispatcher<Destination_t>::Dispatcher()
      : m_thread(std::async(std::launch::async,
                            &My_t::dispatch,
                            this,
                            std::ref(m_exchanger),
                            std::ref(m_stop),
                            std::ref(m_destination)
      )),
      m_scopeGuard{this, [this](void *) { m_stop = true; }}
  {};


  template<typename Destination_t>
  std::weak_ptr<Dispatcher<Destination_t>> Dispatcher<Destination_t>::instanceWeak()
  {
    // "new" needed and no std::make_shared because of private c-tor
    static auto inst = std::shared_ptr<My_t>(new My_t());
    return inst;
  }


  template<typename Destination_t>
  void Dispatcher<Destination_t>::send(Data &&data)
  {
    std::lock_guard<std::mutex> guard(m_exchanger.first);
    if ( m_exchanger.second.size() == m_exchanger.second.capacity() )
      m_exchanger.second.reserve(std::max(static_cast<size_t>(1u), m_exchanger.second.size() * 2));

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
} // namespace StateReportable::core


namespace
{
  void instantiateTemplateFunctions()
  {
    using namespace StateReportable::core;

    Dispatcher<TestDestination>::instanceWeak().lock()->send(ReportLine("", "", "", 0));
  }
} // namespace
