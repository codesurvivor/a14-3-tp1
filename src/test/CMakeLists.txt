init_test_directory()

add_definitions(-DCLOCK_PERIOD_NS=1)

add_executable(traffic_test "traffic.cpp")
target_link_libraries(traffic_test systemc pthread noc)

add_executable(router_test "router.cpp")
target_link_libraries(router_test systemc pthread noc)

add_executable(arbiter_test "arbiter.cpp")
target_link_libraries(arbiter_test systemc pthread noc)

add_executable(noc_test "noc.cpp" "packet_tracer.h" "packet_tracer.cpp")
target_link_libraries(noc_test systemc pthread noc)
