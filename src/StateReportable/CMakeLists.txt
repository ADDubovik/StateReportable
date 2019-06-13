project(StateReportable)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/StateReportable.config" "report.txt")

set(SRC_CORE
	core/calcStat.cpp
	core/calcStat.h
	core/Dispatcher.h
	core/DispatcherImplementation.h
	core/ReportLine.cpp
	core/ReportLine.h
	core/split.cpp
	core/split.h
	core/StatLine.cpp
	core/StatLine.h
	core/StateBase.h
	core/TemplateHelpers.h
	core/VectorHelpers.h
)

set(SRC_TO_FILE
	toFile/Reporter.cpp
	toFile/Reporter.h
	toFile/State.h
	toFile/ConfigReader.cpp
	toFile/ConfigReader.h
)

set(SRC
	${SRC_CORE}
	${SRC_TO_FILE}
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

add_library(${PROJECT_NAME}_lib ${SRC})
add_executable(${PROJECT_NAME}_toFile_demo toFile/demo.cpp)
add_executable(${PROJECT_NAME}_toFile_statMaker toFile/statMaker.cpp)

target_compile_options(${PROJECT_NAME}_toFile_demo PRIVATE -DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_NAME_W=L"${PROJECT_NAME}")
target_compile_options(${PROJECT_NAME}_toFile_statMaker PRIVATE -DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_NAME_W=L"${PROJECT_NAME}")


add_dependencies(
	${PROJECT_NAME}_toFile_demo
	${PROJECT_NAME}_lib
)

add_dependencies(
	${PROJECT_NAME}_toFile_statMaker
	${PROJECT_NAME}_lib
)

target_link_libraries(${PROJECT_NAME}_toFile_demo
	${PROJECT_NAME}_lib
	${ADDITIONAL_LIBS}
)

target_link_libraries(${PROJECT_NAME}_toFile_statMaker
	${PROJECT_NAME}_lib
	${ADDITIONAL_LIBS}
)

set_property(TARGET ${PROJECT_NAME}_lib PROPERTY FOLDER "StateReportable")
set_property(TARGET ${PROJECT_NAME}_toFile_demo PROPERTY FOLDER "StateReportable")
set_property(TARGET ${PROJECT_NAME}_toFile_statMaker PROPERTY FOLDER "StateReportable")

source_group("core"   FILES ${SRC_CORE})
source_group("toFile" FILES ${SRC_TO_FILE})


INSTALL(TARGETS ${PROJECT_NAME}_toFile_demo DESTINATION .)
INSTALL(FILES toFile/StateReportable.config DESTINATION .)
INSTALL(TARGETS ${PROJECT_NAME}_toFile_statMaker DESTINATION .)
