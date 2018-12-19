cmake_minimum_required( VERSION 2.8 )
project( ex3 )

# Add C++ 11 Standard support
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

#list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

find_package( OpenCV 3.3 REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS}
)

# Add an executable file
add_executable( ex3 ex3.cpp filter.cpp )
target_link_libraries( ex3 
   ${OpenCV_LIBS}
)
