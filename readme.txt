To Compile on linux

qmake -project QT+=opengl
qmake
make

To compile on OSX:
Use Homebrew to install qt

qmake -project QT+=opengl
qmake
make

To compile on Windows:
Unfortunately, the official OpenGL on Windows was locked at GL 1.1.  Many many hacks exist, and they all disagree.
Just to make it worse, the Qt response to this is clumsy.  Net result: there is no easy way to get this compiling on Windows.
I will aim to update these instructions at a later date.

To run on linux / feng-gps:

./LoopSubdivisionRelease ../path_to/model.diredgenormal

To run on OSX:
./LoopSubdivisionRelease.app/Contents/MacOS/FakeGLRenderWindowRelease  ../path_to/model.diredgenormal


