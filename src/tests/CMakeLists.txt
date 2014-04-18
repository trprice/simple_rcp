INCLUDE_DIRECTORIES("${TESTS_DIR}")

# Find the *.cpp files and add them to the executable.
ADD_EXECUTABLE (tests tests.c)

ADD_DEPENDENCIES (tests simple_rcp)
ADD_DEPENDENCIES (tests googletest)

TARGET_LINK_LIBRARIES (tests ${LINK_LIBS} ${TEST_LIBS} )

