add_executable(ReEntrantLock_Test ReEntrantLockTests.cpp)

target_link_libraries(ReEntrantLock_Test
  PRIVATE gtest_main ReEntrantLockLib SharedClassLib project_warnings)

include(GoogleTest)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.18)
  # PRE_TEST: list the tests when ctest runs rather than at build time, so
  # builds that can't run the binary yet (e.g. sanitizer builds) still succeed.
  gtest_discover_tests(ReEntrantLock_Test DISCOVERY_MODE PRE_TEST)
else()
  gtest_discover_tests(ReEntrantLock_Test)
endif()
