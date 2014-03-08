SET(NAME isight_snap)

ADD_EXECUTABLE(
  ${NAME}
  main.cpp

  isight.m
  isight.h
  )

TARGET_LINK_LIBRARIES(${NAME} ${OSX_LIBS})