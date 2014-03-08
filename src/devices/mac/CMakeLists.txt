SET(SNAP mac_snapshot)

ADD_EXECUTABLE(
  ${SNAP}
  main.cpp

  video.m
  video.h

  Snapper.m
  )

TARGET_LINK_LIBRARIES(${SNAP} ${MAC_LIBS})
