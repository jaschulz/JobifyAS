project(${PROJECT_NAME} C CXX)

add_library(handlers JobifyController.cpp AccountController.cpp ProfileController.cpp)
target_link_libraries(handlers ${LIBS_NAMES})


