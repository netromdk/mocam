SET(MAC_SOURCE)
IF (APPLE)
  INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/mac
    )

  SET(
    MAC_SOURCE

    mac/Util.h
    mac/Util.m

    mac/VideoDeviceImpl.h
    mac/VideoDeviceImpl.cpp

    mac/CaptureSessionImpl.h
    mac/CaptureSessionImpl.cpp
    )
ENDIF()

SET(NAME snapshot)

ADD_EXECUTABLE(
  ${NAME}
  ${NAME}.cpp

  VideoDevice.h
  VideoDevice.cpp

  CaptureSession.h
  CaptureSession.cpp

  ${MAC_SOURCE}
  )

TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})