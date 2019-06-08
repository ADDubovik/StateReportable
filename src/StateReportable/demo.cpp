#include <atomic>
#include <future>
#include <thread>
#include <chrono>

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


void run(std::atomic<bool> &interrupted)
{
  while ( !interrupted )
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

  {
    auto thread = std::async(std::launch::async, run, std::ref(interrupted));
  } // <--- will hang here until interrupted

  return 0;
}
