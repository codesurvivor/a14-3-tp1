project(systemc)

file(GLOB HEADER
  "include/*.h"
  "include/*/*.h"
  "include/*/*/*.h"
  "include/*/*/*/*.h"
  "include/*.hpp"
  "include/*/*.hpp"
  "include/*/*/*.hpp"
  "include/*/*/*/*.hpp")

file(GLOB SOURCE
  "src/*.cpp"
  "src/*/*.cpp"
  "src/*/*/*.cpp"
  "src/*/*/*/*.cpp"
  "src/*.c"
  "src/*/*.c"
  "src/*/*/*.c"
  "src/*/*/*/*.c")

enable_language(ASM-ATT)
if     ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
  list(APPEND SOURCE "./src/sysc/qt/md/iX86_64.s")
elseif ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "i.86")
  list(APPEND SOURCE "./src/sysc/qt/md/i386.s")
else()
  message(FATAL "Check make the assembly choice corresponding to your platform (${CMAKE_SYSTEM_PROCESSOR}).")
endif()

include_directories("./include" "./src")

add_library(${PROJECT_NAME} ${HEADER} ${SOURCE})

