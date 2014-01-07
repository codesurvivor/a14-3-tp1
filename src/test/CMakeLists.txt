init_test_directory()

file(GLOB HEADER
  "../include/*/*.h"
  "../include/*/*.hpp"
  "./*.h"
  "./*.hpp")
file(GLOB SOURCE "./*.cpp")

set(SIZES_TO_BUILD 8 10 14 64)

foreach(SIZE ${SIZES_TO_BUILD})

  set(CURRENT_TARGET "${PROJECT_NAME}${SIZE}")
  add_executable(${CURRENT_TARGET} ${HEADER} ${SOURCE})
  target_link_libraries(${CURRENT_TARGET} systemc pthread)

  get_target_property(TEMP ${CURRENT_TARGET} COMPILE_FLAGS)
  if(TEMP STREQUAL "TEMP-NOTFOUND")
    set(TEMP "")
  else()
    set(TEMP "${TEMP} ")
  endif()
  set(TEMP "${TEMP}-DFIR_SIZE=${SIZE}")
  set_target_properties(${CURRENT_TARGET} PROPERTIES COMPILE_FLAGS ${TEMP})

endforeach()

