project(${PROJECT_NAME} C CXX)

add_library(Handlers JobifyController.cpp AccountController.cpp ProfileController.cpp SSController.cpp SSHandler.cpp FBHandler.cpp)
target_link_libraries(Handlers db Model utils ${LIBS_NAMES})


