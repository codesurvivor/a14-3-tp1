init_test_directory()

file(GLOB HEADER
  "../../include/*/*.h"
  "../../include/*/*.hpp"
  "./*.h"
  "./*.hpp")
file(GLOB SOURCE "./*.cpp" "../*.cpp")

add_executable(traffic_test ${HEADER} ${SOURCE})
target_link_libraries(traffic_test systemc pthread)

