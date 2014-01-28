init_test_directory()

add_executable(traffic_test traffic.cpp)
target_link_libraries(traffic_test systemc pthread)

add_executable(router_test router.cpp)
target_link_libraries(router_test systemc pthread noc)

