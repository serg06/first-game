# Game.

An attempt to learn 3D graphics from the bottom up.
Following this tutorial: http://learnopengl.com/.
First time in C++, prepare for a mess!

---

To start programming:

- From existing project:
  - Just open solution the .sln file and you should be good! Unless it can't find include/lib shit, then:
    - Project -> properties -> VC++ directories -> include directories -> add the correct include directory
    - Same for lib directories -> add correct lib directory

- From scratch:
  - Change to 64-bit project using http://stackoverflow.com/a/4364020
  - Download GLFW 64-bit src & build with cmake 64-bit
  - Download GLEW 64-bit
  - Copy glfw/include/glfw folder to projectdir/include/
  - Copy glew/include/gl folder to projectdir/include/
  - Add include directory projectdir/include
  - Copy glfw/build/src/Debug/glfw?.lib to projectdir/lib/
  - Copy glew/?/x64/glfw?.lib to projectdir/lib/
  - Add library directory projectdir/lib
  - Add .lib file names to linker/input in vc++ projec settings (including opengl32.lib if ur on windowz)
  - Add glew dll to game.exe folder

- To init glfw and glew:
  - Init glfw
  - Create glfw window
  - Init glew
  - Do not fucking set glew to GLEW_STATIC because that messes up some fucking shit
