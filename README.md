simple_rcp
==========

Simple RCP client/server to learn about socket programming.


Building
--------

Tools required: CMake

1. <code>cd source_directory</code>
2. <code>mkdir build</code>
3. <code>cd build</code>
4. <code>cmake ..</code>
5. <code>make</code>

The resulting binary will be in source_directory/build/bin.


Done
----
Currently the server will start and is setup to accept a connection, receive a
file name, create the file, and receive file data.


TO DO
-----
I need to still write the client side to open a connection, read the file, and
write it to the socket.

Additionally, the unit testing is not turned on yet and no tests have been
written. The framework for writing tests is in place.
