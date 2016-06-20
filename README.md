Sound, Input, and Graphics Integration Library (SIGIL)
------------------------------------------------------

A very simple, cross-platform library for playing audio, handling basic input, and drawing 2D graphics such as sprites, shapes, and text.

Folder Structure:
-----------------

deps: contains the CMake-based dependencies of SIGIL; two of these (GLFW and Freetype2) must be downloaded with Git, but PIGU has been modified to use a CMake-based build system by myself and does not require this

examples: contains stand-alone example projects, with project files/makefiles for every single platform/compiler that SIGIL supports

glew: contains headers and pre-compiled libs for the OpenGL Extension Wrangler library, which SIGIL depends on

include: contains the "sl.h" header file you need to include in your applications to use SIGIL

scripts: contains scripts designed to simplify the mass-build process I use; intended for my use only (but feel free to use it)

sigil-XXXXXX: contains DLLs and static libs for SIGIL built using various compilers

src: SIGIL source code, as well as some source code from some open-source libraries that SIGIL depends on (and are not CMake-based)

Building from Source
--------------------

Conveniently, build instructions are located in this directory with the name BUILD-XXXXXX.txt for every platform/compiler that SIGIL supports.

License Information
-------------------

View the LICENSE.txt file for license information. And dear Christ, is that a dry read.

Contact
-------

Questions, comments, and mildly-rude sarcasm may be directed towards geoff@libsigil.com.
