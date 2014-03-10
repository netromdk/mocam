SET(SDK_MIN "10.8")
SET(SDK "10.9")
SET(DEV_SDK "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${SDK}.sdk")

ADD_DEFINITIONS(
  -DMAC
  -DGCC_VISIBILITY
  -mmacosx-version-min=${SDK_MIN}
  )

SET(CMAKE_OSX_SYSROOT ${DEV_SDK})

MESSAGE("Using SDK: ${DEV_SDK}")

FIND_LIBRARY(FOUNDATION_LIB Foundation)
FIND_LIBRARY(AV_LIB AVFoundation)

MARK_AS_ADVANCED(
  FOUNDATION_LIB 
  AV_LIB
  )

SET(EXTRA_LIBS "${EXTRA_LIBS};${FOUNDATION_LIB};${AV_LIB}")
