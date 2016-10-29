project(${PROJECT_NAME} C CXX)

add_library(utils Encrypt.cpp)
target_link_libraries(utils ${LIBS_NAMES})


