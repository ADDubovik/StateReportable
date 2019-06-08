project(StateReportable)

set(SRC_CORE
	core/Dispatcher.cpp
	core/Dispatcher.h
	core/ReportLine.cpp
	core/ReportLine.h
	core/split.cpp
	core/split.h
	core/StateBase.h
	core/TemplateHelpers.h
	core/TestDestination.cpp
	core/TestDestination.h
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
add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_NAME_W=L"${PROJECT_NAME}")


add_dependencies(
	${PROJECT_NAME}
	${PROJECT_NAME}_lib
)

target_link_libraries(${PROJECT_NAME}
	${PROJECT_NAME}_lib
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "StateReportable")
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY FOLDER "StateReportable")

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION .)
