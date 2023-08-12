# set(BINARY ${CMAKE_PROJECT_NAME})

# file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

# set(SOURCES ${SOURCES})

# add_executable(${BINARY}_run ${SOURCES})

# add_library(${BINARY}_lib STATIC ${SOURCES})

project(ReEntrantLockApp)

set(APP_SOURCES src/main.cpp)

set(SOURCES ${APP_SOURCES})

add_executable(ReEntrantLockApp ${APP_SOURCES})

target_link_libraries(ReEntrantLockApp PRIVATE ReEntrantLockLib)