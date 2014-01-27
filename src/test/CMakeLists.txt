init_test_directory()

add_executable(traffic_test traffic.cpp)
target_link_libraries(traffic_test systemc pthread)

