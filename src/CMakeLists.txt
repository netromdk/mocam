SET(MAC_SOURCE)
IF (APPLE)
  ADD_SUBDIRECTORY(mac)
  INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/mac
    )

  SET(
    MAC_SOURCE

    mac/Util.h
    mac/Util.m

    mac/ApplicationImpl.h
    mac/ApplicationImpl.cpp

    mac/VideoDeviceImpl.h
    mac/VideoDeviceImpl.cpp
    )
ENDIF()

SET(NAME snapshot)

ADD_EXECUTABLE(
  ${NAME}
  ${NAME}.cpp

  Application.h
  Application.cpp

  VideoDevice.h
  VideoDevice.cpp

  ${MAC_SOURCE}
  )

TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})