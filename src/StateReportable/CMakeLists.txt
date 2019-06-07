project(StateReportable)

set(SRC
	ReportLine.cpp
	ReportLine.h
	split.cpp
	split.h
	StateBase.h
	StateToFile.h
	StateToFileConfig.cpp
	StateToFileConfig.h
	TemplateHelpers.h
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
