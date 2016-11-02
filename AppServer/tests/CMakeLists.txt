add_executable(ProfileTest ProfileTest.cpp main.cpp)
target_link_libraries(ProfileTest Model utils ${LIBS_NAMES})

add_executable(AccountControllerTest AccountControllerTest.cpp ${MONGOOSE_SOURCES} JobifyControllerTest.cpp main.cpp)
target_link_libraries(AccountControllerTest Model utils db Handlers mongoose ${LIBS_NAMES})

add_test(NAME "AccountControllerTest" COMMAND AccountControllerTest)
add_test(NAME "ProfileTest" COMMAND ProfileTest)
