simple_rcp
==========

Simple RCP client/server to learn about socket programming.


Necessary Tools
--------

1. git - to clone from github
2. CMake - to generate the makefiles
3. svn - to download gtest


Building
--------

1. <code>cd source_directory</code>
2. <code>mkdir build</code>
3. <code>cd build</code>
4. <code>cmake ..</code>
5. <code>make</code>

CMake will generate make files using the C and C++ compilers set in the
CC and CXX environment variables, respectively.

The resulting binary will be in source_directory/build/bin.


Done
----
Currently the server will start and is setup to accept a connection, receive a
file name, create the file, and receive file data.


TO DO
-----
Additionally, the unit testing is not turned on yet and no tests have been
written. The framework for writing tests is in place and will be downloaded
as part of the build.
