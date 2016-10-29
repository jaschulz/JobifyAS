project(${PROJECT_NAME} C CXX)

add_library(db dbController.cpp dbCredentials.cpp dbUsers.cpp)
target_link_libraries(db ${LIBS_NAMES})


