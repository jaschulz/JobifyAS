project(${PROJECT_NAME} C CXX)

add_library(Model Profile.cpp Profile.h Location.cpp Location.h Credentials.cpp Credentials.h fbCredentials.cpp fbCredentials.h jobifyCredentials.cpp jobifyCredentials.h)
target_link_libraries(Model utils ${LIBS_NAMES})


