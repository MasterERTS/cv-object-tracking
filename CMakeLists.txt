cmake_minimum_required(VERSION 2.8.3)
project( tracking )
find_package( OpenCV REQUIRED )
add_executable( tracking src/tracking.cpp )
target_link_libraries( tracking ${OpenCV_LIBS} )


