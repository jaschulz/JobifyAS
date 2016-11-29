project(${PROJECT_NAME} C CXX)

add_library(Model Profile.cpp Profile.h Location.cpp Location.h Credentials.cpp Credentials.h fbCredentials.cpp fbCredentials.h jobifyCredentials.cpp jobifyCredentials.h Category.h Category.cpp Entity.h Entity.cpp Experience.h Experience.cpp ExpMin.h ExpMin.cpp)
target_link_libraries(Model utils ${LIBS_NAMES})


