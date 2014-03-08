SET(NAME mac_snapper)

ADD_EXECUTABLE(
  ${NAME}
  main.cpp

  video.m
  video.h

  Snapper.m
  )

TARGET_LINK_LIBRARIES(${NAME} ${EXTRA_LIBS})
