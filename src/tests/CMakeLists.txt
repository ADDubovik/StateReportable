enable_testing()

set(GTest_DIR ${CMAKE_SOURCE_DIR}/3rdparty/googletest)
set(GTest_INCLUDE_DIR ${GTest_DIR}/googletest/include)
set(GTest_ADD_TO_SRC  ${GTest_DIR}/googletest/src/gtest-all.cc)


include_directories(
	${GTest_INCLUDE_DIR}
	${GTest_INCLUDE_DIR}/..
	${CMAKE_SOURCE_DIR}/src/StateReportable
	${CMAKE_SOURCE_DIR}/src/tests
)

set(GTEST_ROOT ${GTest_DIR}/googletest)

find_package(GTest)


macro(STATE_REPORTABLE_ADD_TEST testName)
	add_executable(
		${testName}
		${ARGN}
		main.h
		${GTest_ADD_TO_SRC}
	)
	GTEST_ADD_TESTS(${testName} "" ${ARGN})
	set_property(TARGET ${testName} PROPERTY FOLDER "Tests/Auto")
	target_compile_options(${testName} PRIVATE -DPROJECT_NAME="AutoTests" -DPROJECT_NAME_W=L"AutoTests")
endmacro()

add_definitions(
	-DGTEST_LANG_CXX11
)

#message(STATUS ADDITIONAL_LIBS ${ADDITIONAL_LIBS})

STATE_REPORTABLE_ADD_TEST(ReportLineTest ReportLineTest/ReportLineTest.cpp) 
  target_link_libraries(ReportLineTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(splitTest splitTest/splitTest.cpp)
  target_link_libraries(splitTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(StateBaseTest StateBaseTest/StateBaseTest.cpp) 
  target_link_libraries(StateBaseTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(StateToFileConfigReaderTest StateToFileConfigReaderTest/StateToFileConfigReaderTest.cpp) 
  target_link_libraries(StateToFileConfigReaderTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(ThreadSyncConceptTest ThreadSyncConceptTest/ThreadSyncConceptTest.cpp) 
  target_link_libraries(ThreadSyncConceptTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(DispatcherTest DispatcherTest/DispatcherTest.cpp) 
  target_link_libraries(DispatcherTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(DispatcherWeakSingletonTest DispatcherWeakSingletonTest/DispatcherWeakSingletonTest.cpp) 
  target_link_libraries(DispatcherWeakSingletonTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(StatLineTest StatLineTest/StatLineTest.cpp)
  target_link_libraries(StatLineTest StateReportable_lib ${ADDITIONAL_LIBS})
STATE_REPORTABLE_ADD_TEST(calcStatTest calcStatTest/calcStatTest.cpp) 
  target_link_libraries(calcStatTest StateReportable_lib ${ADDITIONAL_LIBS})
