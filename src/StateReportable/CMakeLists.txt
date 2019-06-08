project(StateReportable)

set(SRC_CORE
	core/Dispatcher.h
	core/DispatcherImplementation.h
	core/ReportLine.cpp
	core/ReportLine.h
	core/split.cpp
	core/split.h
	core/StateBase.h
	core/TemplateHelpers.h
)

set(SRC_TO_FILE
	ReporterToFile.cpp
	ReporterToFile.h
	StateToFile.cpp
	StateToFile.h
	StateToFileConfigReader.cpp
	StateToFileConfigReader.h
)

set(SRC
	${SRC_CORE}
	${SRC_TO_FILE}
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME}_lib ${SRC})
add_executable(${PROJECT_NAME}_demo demo.cpp)

target_compile_options(${PROJECT_NAME}_demo PRIVATE -DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_NAME_W=L"${PROJECT_NAME}")


add_dependencies(
	${PROJECT_NAME}_demo
	${PROJECT_NAME}_lib
)

target_link_libraries(${PROJECT_NAME}_demo
	${PROJECT_NAME}_lib
)

set_property(TARGET ${PROJECT_NAME}_demo PROPERTY FOLDER "StateReportable")
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY FOLDER "StateReportable")

INSTALL(TARGETS ${PROJECT_NAME}_demo DESTINATION .)
