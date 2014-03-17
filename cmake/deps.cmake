# Qt 5
SET(CMAKE_AUTOMOC ON) # Moc when necessary.

# As moc files are generated in the binary dir, tell CMake to always
# look for includes there:
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

# OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
