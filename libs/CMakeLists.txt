if(BUILD_TESTING)
  # Use the shared C runtime on MSVC so googletest matches the rest of the project.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  add_subdirectory(googletest)
endif()

add_subdirectory(Lock)
add_subdirectory(ReEntrantLock)
add_subdirectory(SharedClass)
