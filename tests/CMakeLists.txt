project(ReEntrantLockTest)

set(TEST_SOURCES main.cpp ReEntrantLockTests.cpp)

add_executable(ReEntrantLockTest ${TEST_SOURCES})

add_test(NAME ReEntrantLockTest COMMAND ReEntrantLockTest)

target_link_libraries(ReEntrantLockTest PUBLIC gtest)
target_link_libraries(ReEntrantLockTest PRIVATE ReEntrantLockLib)