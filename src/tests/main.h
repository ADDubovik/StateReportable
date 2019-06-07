#ifdef _WIN32
#include <Windows.h>
#include <crtdbg.h>
#endif

int main(int ac, char *av[])
{
#ifdef _WIN32
  if( !IsDebuggerPresent() )
  {
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
  }
#endif

  testing::InitGoogleTest(&ac, av);

#ifdef GMOCK_INCLUDE_GMOCK_GMOCK_H_
  testing::InitGoogleMock(&ac, av);
#endif // GMOCK_INCLUDE_GMOCK_GMOCK_H_

  return RUN_ALL_TESTS();
}
