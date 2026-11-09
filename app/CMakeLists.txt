add_executable(ReEntrantLockApp src/main.cpp)

target_link_libraries(ReEntrantLockApp
  PRIVATE ReEntrantLockLib SharedClassLib project_warnings)
