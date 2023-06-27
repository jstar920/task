file (GLOB TASK_HEADERS "*.h")
file (GLOB TASK_SOURCES "*.cpp")

message(STATUS "header files:" ${TASK_HEADERS})
message(STATUS "source files:" ${TASK_SOURCES})

source_group("headers" FILES ${TASK_HEADERS})
source_group("sources" FILES ${TASK_SOURCES})

include_directories (.)

add_library(task SHARED ${TASK_HEADERS} ${TASK_SOURCES})
add_library(task_static STATIC ${TASK_HEADERS} ${TASK_SOURCES})

set_property(TARGET task PROPERTY FOLDER ${PRJECT_LIBRARIES_DIR})
set_property(TARGET task_static PROPERTY FOLDER ${PRJECT_LIBRARIES_DIR})
target_include_directories(task_static PUBLIC ".")

install (TARGETS task
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install
)
