project(${PROJECT_NAME} C CXX)

add_library(Model Profile.cpp Profile.h)
target_link_libraries(Model utils ${LIBS_NAMES})


