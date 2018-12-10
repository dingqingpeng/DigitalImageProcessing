cmake_minimum_required( VERSION 2.8 )
project( EX2 )

# Add C++ 11 Standard support
set( CMAKE_CXX_FLAGS "-std=c++11" )

# OpenCV include directory
find_package( OpenCV 3.3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add an executable file
add_executable( trans ex2.cpp transform.cpp )
target_link_libraries( trans ${OpenCV_LIBS} )
