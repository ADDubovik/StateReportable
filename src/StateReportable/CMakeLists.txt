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
	core/StatLine.h
	core/StateBase.h
	core/TemplateHelpers.h
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
)

add_library(${PROJECT_NAME}_lib ${SRC})
add_executable(${PROJECT_NAME}_toFile_demo toFile/demo.cpp)

target_compile_options(${PROJECT_NAME}_toFile_demo PRIVATE -DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_NAME_W=L"${PROJECT_NAME}")


add_dependencies(
	${PROJECT_NAME}_toFile_demo
	${PROJECT_NAME}_lib
)

target_link_libraries(${PROJECT_NAME}_toFile_demo
	${PROJECT_NAME}_lib
)

set_property(TARGET ${PROJECT_NAME}_toFile_demo PROPERTY FOLDER "StateReportable")
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY FOLDER "StateReportable")

source_group("core"   FILES ${SRC_CORE})
source_group("toFile" FILES ${SRC_TO_FILE})


INSTALL(TARGETS ${PROJECT_NAME}_toFile_demo DESTINATION .)
