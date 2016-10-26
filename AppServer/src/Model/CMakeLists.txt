project(${PROJECT_NAME} C CXX)

add_library(model Profile.cpp Profile.h)
target_link_libraries(model ${LIBS})


