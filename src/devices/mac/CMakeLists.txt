SET(NAME snapshot)

ADD_EXECUTABLE(
  ${NAME}
  main.cpp

  video.m
  video.h

  Snapper.m
  )

TARGET_LINK_LIBRARIES(${NAME} ${MAC_LIBS})
