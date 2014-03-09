SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-O4 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Note: If -ObjC++ is not used then -std=c++11 will not work when
# mixing C++/ObjC on GCC/CLang.

IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  EXECUTE_PROCESS(
    COMMAND
    ${CMAKE_CXX_COMPILER} -dumpversion
    OUTPUT_VARIABLE GCC_VERSION
    )
  IF (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
    MESSAGE(FATAL_ERROR "Requires GCC >= 4.7.")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ObjC++")
  ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  EXECUTE_PROCESS(
    COMMAND
    ${CMAKE_CXX_COMPILER} -dumpversion
    OUTPUT_VARIABLE CLANG_VERSION
    )
  IF (NOT (CLANG_VERSION VERSION_GREATER 3.2 OR GCC_VERSION VERSION_EQUAL 3.2))
    MESSAGE(FATAL_ERROR "Requires Clang >= 3.2.")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -ObjC++")
  ENDIF()
ELSEIF (MSVC AND (MSVC10 OR MSVC11 OR MSVC12))
  # C++11 support is implicitly enabled.
ELSE()
  MESSAGE(FATAL_ERROR "Your compiler does not support C++11 - aborting!")
ENDIF()