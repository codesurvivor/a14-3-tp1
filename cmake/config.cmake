
macro(ADD_TEST_DIRECTORY _directory)
  if (BUILD_TESTS)
    add_subdirectory(${_directory})
  endif()
endmacro()

macro(INIT_TEST_DIRECTORY)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/test)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/test)
endmacro()

