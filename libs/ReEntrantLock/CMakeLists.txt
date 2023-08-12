project(ReEntrantLockLib)

add_library(ReEntrantLockLib src/ReEntrantLock.cpp)

target_include_directories(ReEntrantLockLib
	PUBLIC
		include
	PRIVATE
		src
)