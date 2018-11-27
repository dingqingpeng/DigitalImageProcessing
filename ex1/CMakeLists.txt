cmake_minimum_required( VERSION 2.8 )
project( bmpread )

# Add C++ 11 Standard support
set( CMAKE_CXX_FLAGS "-std=c++11" )

# OpenCV include directory
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add an executable file
add_executable( bmpread bitmap.cpp bmpFuncs.cpp )
target_link_libraries( bmpread ${OpenCV_LIBS} )
