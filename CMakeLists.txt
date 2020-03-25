cmake_minimum_required(VERSION 2.8.3)
project( tracking )
find_package( OpenCV REQUIRED )
add_executable( tracking src/tracking.cpp )
target_link_libraries( tracking ${OpenCV_LIBS} )
add_executable( tracking_previous src/tracking_previous.cpp )
target_link_libraries( tracking_previous ${OpenCV_LIBS} )
add_executable( tracking_custom src/tracking_roi.cpp )
target_link_libraries( tracking_custom ${OpenCV_LIBS} )
add_executable( tracking_webcam src/tracking_webcam.cpp )
target_link_libraries( tracking_webcam ${OpenCV_LIBS} )


