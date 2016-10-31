add_executable(ProfileTest ProfileTest.cpp main.cpp)
target_link_libraries(ProfileTest Model utils ${LIBS})

add_test(NAME "ProfileTest" COMMAND ProfileTest)
