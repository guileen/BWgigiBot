cmake_minimum_required(VERSION 3.9)
project(MyAIModule)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
if (BUILD_COMPAT)
	if (NOT MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
	endif()
endif()

include_directories(
  ../bwapi/bwapi/include 
  deps/liblmdb
)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
LINK_DIRECTORIES(../bwapi/build/lib)

file(GLOB AI_SRC
	"Source/*.h"
	"Source/*.cpp"
	"Source/bwsim/*.h"
	"Source/bwsim/*.cpp"
)

add_library(MyAIModule SHARED
	${AI_SRC}
  )

target_link_libraries(MyAIModule BWAPILIB)
