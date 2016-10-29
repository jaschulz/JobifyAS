project(${PROJECT_NAME} C CXX)

add_library(Handlers JobifyController.cpp AccountController.cpp ProfileController.cpp SSController.cpp SSHandler.cpp)
target_link_libraries(Handlers db Model utils jsoncpp ${LIBS_NAMES})


