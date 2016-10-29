project(${PROJECT_NAME} C CXX)

add_library(Handlers JobifyController.cpp AccountController.cpp ProfileController.cpp)
target_link_libraries(Handlers ${LIBS_NAMES})


