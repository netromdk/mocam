### snapshot ###
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

  VideoDevice.h
  VideoDevice.cpp

  CaptureSession.h
  CaptureSession.cpp

  ${MAC_SOURCE}
  )

QT5_USE_MODULES(${NAME} Core Gui)
TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})

### faces ###
SET(MAC_SOURCE)

SET(NAME faces)

ADD_EXECUTABLE(
  ${NAME}
  ${NAME}.cpp

  Types.h
  Types.cpp

  Util.h
  Util.cpp

  Face.h
  Face.cpp

  FaceDetector.h
  FaceDetector.cpp

  ${MAC_SOURCE}
  )

QT5_USE_MODULES(${NAME} Core Gui)
TARGET_LINK_LIBRARIES(
  ${NAME} 
  ${EXTRA_LIBS} 
  ${OpenCV_LIBS}
  )

### mocam ###
SET(MAC_SOURCE)
IF (APPLE)
  SET(
    MAC_SOURCE

    mac/MacUtil.h
    mac/MacUtil.m

    mac/VideoDeviceImpl.h
    mac/VideoDeviceImpl.cpp

    mac/CaptureSessionImpl.h
    mac/CaptureSessionImpl.cpp

    mac/FrameGrabber.h
    mac/FrameGrabber.cpp
    )
ENDIF()

SET(NAME mocam)

ADD_EXECUTABLE(
  ${NAME}
  ${NAME}.cpp

  Types.h
  Types.cpp

  Window.h
  Window.cpp

  VideoDevice.h
  VideoDevice.cpp

  CaptureSession.h
  CaptureSession.cpp

  ${MAC_SOURCE}
  )

QT5_USE_MODULES(${NAME} Core Gui Widgets)
TARGET_LINK_LIBRARIES(
  ${NAME} 
  ${EXTRA_LIBS} 
  ${OpenCV_LIBS}
  )