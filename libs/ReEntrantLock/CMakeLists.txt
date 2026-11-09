add_library(ReEntrantLockLib src/ReEntrantLock.cpp)

target_include_directories(ReEntrantLockLib PUBLIC include)

target_link_libraries(ReEntrantLockLib
  PUBLIC LockLib Threads::Threads
  PRIVATE project_warnings)
