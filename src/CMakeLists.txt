project("noc")

include_directories(".")

file(GLOB HEADER "../include/*/*.h")
file(GLOB SOURCE "*.cpp")

add_library(${PROJECT_NAME} ${HEADER} ${SOURCE})
target_link_libraries(${PROJECT_NAME} systemc)

add_test_directory(test)
