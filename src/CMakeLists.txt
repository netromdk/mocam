SET(MAC_SOURCE)
IF (APPLE)
  INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/mac
    )

  SET(
    MAC_SOURCE

    mac/MacUtil.h
    mac/MacUtil.m

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

  Util.h
  Util.cpp

  VideoDevice.h
  VideoDevice.cpp

  CaptureSession.h
  CaptureSession.cpp

  ${MAC_SOURCE}
  )

QT5_USE_MODULES(${NAME} Core Gui)
TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})