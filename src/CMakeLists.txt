IF (APPLE)
  ADD_SUBDIRECTORY(mac)
ENDIF()

SET(NAME snapshot)

ADD_EXECUTABLE(
  ${NAME}
  ${NAME}.cpp

  VideoDevice.h
  CaptureSession.h
  )

TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})