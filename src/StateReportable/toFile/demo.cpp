#include "toFile/State.h"

#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>


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
  using StateReportableHttps = StateReportable::toFile::State<connection::https_state>;
  using StateReportableCalculating = StateReportable::toFile::State<calculating::calc_state>;

  using HttpsUnique = std::unique_ptr<StateReportableHttps>;
  using CalculatingUnique = std::unique_ptr<StateReportableCalculating>;

  HttpsUnique https;
  CalculatingUnique calc;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> resetDistribution(0, 0xfff);
  std::uniform_int_distribution<> nextDistribution(0, 0xf);

  while ( !interrupted )
  {
    {
      if ( !https )
        https = std::make_unique<StateReportableHttps>(connection::getFirst());

      if ( https && (resetDistribution(gen) == 0) )
        https.reset();

      if ( https && (nextDistribution(gen) == 0) )
        *https = connection::next(*https);

      if ( https && connection::isLast(*https) )
        https.reset();
    }

    {
      if ( !calc )
        calc = std::make_unique<StateReportableCalculating>(calculating::getFirst());

      if ( calc && (resetDistribution(gen) == 0) )
        calc.reset();

      if ( calc && (nextDistribution(gen) == 0) )
        *calc = calculating::next(*calc);

      if ( calc && calculating::isLast(*calc) )
        calc.reset();
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

  const auto threadsCount = 20;
  {
    std::vector<std::future<void>> threads(threadsCount);

    for ( auto &elem : threads )
      elem = std::async(std::launch::async, run, std::ref(interrupted));
  } // <--- will hang here until interrupted

  return 0;
}
