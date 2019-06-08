#include "StateToFile.h"

#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <memory>

// Ctrl-C flag
static std::atomic<bool> interrupted = false;


#ifdef _WIN32
#include <windows.h>
BOOL WINAPI consoleHandler(DWORD)
{
  interrupted = true;
  return TRUE;
}
#else
#include <csignal>
void consoleHandler(int s)
{
  interrupted = true;
}
#endif


namespace connection
{
  // Sample enum
  enum class https_state
  {
    init,
    allowed,
    resolved,
    tcpReady,
    sslReady,
    wrote,
    read
  };

  std::ostream &operator<<(std::ostream &stream, const https_state state)
  {
    switch (state)
    {
      case https_state::init: return stream << "init";
      case https_state::allowed: return stream << "allowed";
      case https_state::resolved: return stream << "resolved";
      case https_state::tcpReady: return stream << "tcpReady";
      case https_state::sslReady: return stream << "sslReady";
      case https_state::wrote: return stream << "wrote";
      case https_state::read: return stream << "read";
      default : return stream << "unknown";
    };
  }

  https_state getFirst() { return https_state::init; }

  https_state getLast() { return https_state::read; }

  bool isLast(https_state state) { return state == getLast(); }

  https_state next(https_state state)
  {
    return isLast(state) ? getLast() : static_cast<https_state>(static_cast<int>(state) + 1);
  }
} // namespace connection


namespace calculating
{
  // Sample enum
  enum class calc_state
  {
    started,
    dataGot,
    calculated,
    storedToDisk
  };

  std::ostream &operator<<(std::ostream &stream, const calc_state state)
  {
    switch (state)
    {
      case calc_state::started: return stream << "started";
      case calc_state::dataGot: return stream << "dataGot";
      case calc_state::calculated: return stream << "calculated";
      case calc_state::storedToDisk: return stream << "storedToDisk";
      default : return stream << "unknown";
    };
  }

  calc_state getFirst() { return calc_state::started; }

  calc_state getLast() { return calc_state::storedToDisk; }

  bool isLast(calc_state state) { return state == getLast(); }

  calc_state next(calc_state state)
  {
    return isLast(state) ? getLast() : static_cast<calc_state>(static_cast<int>(state) + 1);
  }
} // namespace calculating


void run(std::atomic<bool> &interrupted)
{
  using StateReportableHttps = StateReportable::StateToFile<connection::https_state>;
  using StateReportableCalculating = StateReportable::StateToFile<calculating::calc_state>;

  using HttpsUnique = std::unique_ptr<StateReportableHttps>;
  using CalculatingUnique = std::unique_ptr<StateReportableCalculating>;

  std::vector<HttpsUnique> httpss(20);
  std::vector<CalculatingUnique> calcs(20);

  while ( !interrupted )
  {
    for ( auto &elem : httpss )
    {
      if ( !elem )
        elem = std::make_unique<StateReportableHttps>(connection::getFirst());

      if ( elem && ((rand() & 0xfff) == 0) )
        elem.reset();

      if ( elem && ((rand() & 0xf) == 0) )
        *elem = connection::next(*elem);

      if ( elem && connection::isLast(*elem) )
        elem.reset();
    }

    for ( auto &elem : calcs )
    {
      if ( !elem )
        elem = std::make_unique<StateReportableCalculating>(calculating::getFirst());

      if ( elem && ((rand() & 0xfff) == 0) )
        elem.reset();

      if ( elem && ((rand() & 0xf) == 0) )
        *elem = calculating::next(*elem);

      if ( elem && calculating::isLast(*elem) )
        elem.reset();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}


int main()
{
#ifdef _WIN32
  // For Ctrl-C processing
  SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
  struct sigaction sigIntHandler { {consoleHandler}};
  sigemptyset(&sigIntHandler.sa_mask);
  sigaction(SIGINT, &sigIntHandler, nullptr);
#endif

  std::cout << "Writing demo StateReportable data with accordance to StateReportable.config" << std::endl;
  std::cout << "Press Ctrl-C to exit" << std::endl;

  {
    auto thread = std::async(std::launch::async, run, std::ref(interrupted));
  } // <--- will hang here until interrupted

  return 0;
}
