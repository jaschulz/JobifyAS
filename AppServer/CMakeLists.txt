cmake_minimum_required (VERSION 2.6)
project (appServer)
find_package (Threads)

# - use ISO C++11 required for RocksDB
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -lz -lrt -pthread -std=c++11 -g -fno-strict-aliasing")
set(WARNINGS "-Wall -Wextra -Wunused-parameter -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code")
set(LD_FLAGS "${LD_FLAGS} -Wl,-z,defs")

set (MONGOOSE_CPP "${PROJECT_SOURCE_DIR}/src/mongoose")

set (SOURCES
${SOURCES}
${MONGOOSE_CPP}/Utils.cpp
${MONGOOSE_CPP}/Controller.cpp
${MONGOOSE_CPP}/Mutex.cpp
${MONGOOSE_CPP}/mongoose.c
${MONGOOSE_CPP}/Request.cpp
${MONGOOSE_CPP}/Response.cpp
${MONGOOSE_CPP}/Server.cpp
${MONGOOSE_CPP}/Session.cpp
${MONGOOSE_CPP}/Sessions.cpp
${MONGOOSE_CPP}/StreamResponse.cpp
${MONGOOSE_CPP}/UploadFile.cpp
${MONGOOSE_CPP}/WebController.cpp
${MONGOOSE_CPP}/jsoncpp.cpp
${MONGOOSE_CPP}/JsonResponse.cpp
${MONGOOSE_CPP}/JsonController.cpp
${MONGOOSE_CPP}/WebSocket.cpp
${MONGOOSE_CPP}/WebSockets.cpp)

find_package(Boost REQUIRED COMPONENTS random chrono system thread regex filesystem)
set(Regex_LIBRARIES ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_ATOMIC_LIBRARY}
        ${Boost_CHRONO_LIBRARY}
        ${Boost_RANDOM_LIBRARY}
        ${Boost_REGEX_LIBRARY}
        ${Boost_FRAMEWORK})

include_directories ("${MONGOOSE_CPP}")

include_directories(${Boost_INCLUDE_DIR})
include_directories ("${PROJECT_SOURCE_DIR}")

add_definitions("-DENABLE_STATS")
add_definitions("-DNO_WEBSOCKET")

set (EXTRA_LIBS ${EXTRA_LIBS} dl)


# Compiling library
add_library (mongoose ${SOURCES})
target_link_libraries (mongoose ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})

set (MYSOURCES "${PROJECT_SOURCE_DIR}/src")
file(GLOB ${MYSOURCES}
	"*.h"
	"*.cpp"
	"db/*.h"
	"db/*.cpp"
	"Handlers/*.h"
	"Handlers/*.cpp"
	"Model/*.h"
	"Model/*.cpp"
	"utils/*.h"
	"utils/*.cpp"
)

# Compiling executable
file(GLOB MYSOURCES
    	src/db/*.h
	src/db/*.cpp
	src/Handlers/*.h
	src/Handlers/*.cpp
	src/Model/*.h
	src/Model/*.cpp
	src/utils/*.h
	src/utils/*.cpp
	src/*.h
	src/*.cpp
)

add_executable(main ${MYSOURCES})
#add_executable (main src/main.cpp src/JobifyController.cpp src/JobifyController.h src/SSHandler.cpp src/SSHandler.h)
target_link_libraries (main mongoose ${Casablanca_LIBRARIES} -L/usr/lib/x86_64-linux-gnu -lcurl -lleveldb -lpthread -lsnappy -lcrypto -Wl,-Bsymbolic-functions -Wl,-z,relro)

# install
set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
configure_file(AppServerConfig.cmake.in AppServerConfig.cmake @ONLY)

install (FILES src/mongoose/mongoose.h DESTINATION "${INCLUDE_INSTALL_DIR}")
install (DIRECTORY src/mongoose DESTINATION "${INCLUDE_INSTALL_DIR}" PATTERN "*.cpp" EXCLUDE)
install (TARGETS mongoose DESTINATION lib EXPORT mongoose-targets)
install (EXPORT mongoose-targets DESTINATION "lib/cmake/mongoose" FILE MongooseTargets.cmake)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/MongooseConfig.cmake DESTINATION "lib/cmake/mongoose")
