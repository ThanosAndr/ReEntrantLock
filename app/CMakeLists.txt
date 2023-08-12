project(ReEntrantLockApp)

set(APP_SOURCES src/main.cpp)

set(SOURCES ${APP_SOURCES})

add_executable(ReEntrantLockApp ${APP_SOURCES})

target_link_libraries(ReEntrantLockApp PRIVATE ReEntrantLockLib)