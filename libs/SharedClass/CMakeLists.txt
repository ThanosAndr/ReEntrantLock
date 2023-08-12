project(SharedClassLib)

add_library(SharedClassLib INTERFACE)

target_include_directories(SharedClassLib INTERFACE include)

target_link_libraries(ReEntrantLockApp PUBLIC LockLib)