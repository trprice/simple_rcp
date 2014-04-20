INCLUDE_DIRECTORIES("${TESTS_DIR}")

# Find the *.cpp files and add them to the executable.
ADD_EXECUTABLE (tests tests.cpp parse_args_tests.cpp)

ADD_DEPENDENCIES (tests simple_rcp)
ADD_DEPENDENCIES (tests googletest)

TARGET_LINK_LIBRARIES (tests ${TEST_LIBS} )

