IF (APPLE)
  INCLUDE(mac)
ENDIF()

IF (NOT WIN32)
  # TODO: Find out how to enable only for .cpp files and not .m
  ADD_DEFINITIONS(
    #-std=c++11
    )
ENDIF()