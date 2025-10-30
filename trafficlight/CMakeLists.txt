add_executable(trafficlight main.cpp trafficlight.cpp)
target_include_directories(trafficlight PRIVATE ../include)
target_link_libraries(trafficlight ${MP2_LIBRARY})