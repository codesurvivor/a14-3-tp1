
message(STATUS "Searching for SimSoc")

# The HINTS option should only be used for values computed from the system.
SET(_SIMSOC_HINTS
  ${SIMSOC_PREFIX}
  ${SIMSOC_PREFIX}/include
  ${SIMSOC_PREFIX}/lib
  ${SIMSOC_PREFIX}/libsimsoc
  $ENV{SIMSOC_PREFIX}
  $ENV{SIMSOC_PREFIX}/include
  $ENV{SIMSOC_PREFIX}/lib
  $ENV{SIMSOC_PREFIX}/libsimsoc
  ${CMAKE_INSTALL_PREFIX}
  ${CMAKE_INSTALL_PREFIX}/include
  ${CMAKE_INSTALL_PREFIX}/lib
  ${CMAKE_INSTALL_PREFIX}/libsimsoc
  )

# Hard-coded guesses should still go in PATHS. This ensures that the user
# environment can always override hard guesses.
SET(_SIMSOC_PATHS
  /usr/include/libsimsoc
  /usr/lib
  /usr/local/include/libsimsoc
  /usr/local/lib
  )

FIND_FILE(_SIMSOC_VERSION_FILE
  NAMES simsoc-config.h
  HINTS ${_SIMSOC_HINTS}
  PATHS ${_SIMSOC_PATHS}
  PATH_SUFFIXES ./include/libsimsoc
)

EXEC_PROGRAM("cat ${_SIMSOC_VERSION_FILE} |grep '#define SIMSOC_PACKAGE_STRING' | sed 's/^[^\"]*\"simsoc \\([0-9]*\\).*/\\1/' "
             OUTPUT_VARIABLE SimSoc_MAJOR)
EXEC_PROGRAM("cat ${_SIMSOC_VERSION_FILE} |grep '#define SIMSOC_PACKAGE_STRING' | cut -d '.' -f 2 "
             OUTPUT_VARIABLE SimSoc_MINOR)
EXEC_PROGRAM("cat ${_SIMSOC_VERSION_FILE} |grep '#define SIMSOC_PACKAGE_STRING' | sed 's/.*\\.\\([0-9]*\\)\"$/\\1/' "
             OUTPUT_VARIABLE SimSoc_REV)

set(SimSoc_VERSION ${SimSoc_MAJOR}.${SimSoc_MINOR}.${SimSoc_REV})

if("${SimSoc_MAJOR}" MATCHES "2")
  set(SimSoc_FOUND TRUE)
endif("${SimSoc_MAJOR}" MATCHES "2")

message(STATUS "SimSoc version = ${SimSoc_VERSION}")

FIND_PATH(SimSoc_INCLUDE_DIRS
  NAMES simsoc-config.h
  HINTS ${_SIMSOC_HINTS}
  PATHS ${_SIMSOC_PATHS}
)

FIND_PATH(SimSoc_LIBRARY_DIRS
  NAMES libsimsoc.a
  HINTS ${_SIMSOC_HINTS}
  PATHS ${_SIMSOC_PATHS}
)

set(SimSoc_LIBRARIES ${SimSoc_LIBRARY_DIRS}/libsimsoc.a)

message(STATUS "SimSoc library = ${SimSoc_LIBRARIES}")

