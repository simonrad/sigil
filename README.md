Sound, Input, and Graphics Integration Library (SIGIL)
------------------------------------------------------

A simple, cross-platform library for playing audio, handling basic input, and drawing 2D graphics such as sprites, shapes, and text.

Folder Structure:
-----------------

deps: contains the CMake-based dependencies of SIGIL; if you're planning on building from source, you'll need to consult GET-DEPS.txt in this directory to see how to download some of them

examples: contains stand-alone example projects, with project files/makefiles for every single platform/compiler that SIGIL supports

glew: contains headers and pre-compiled libs for the OpenGL Extension Wrangler library, which SIGIL depends on

glfw: contains headers and pre-compiled libs for the GLFW library, which SIGIL depends on

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

Questions, comments, and mildly-rude sarcasm may be directed towards Geoff Nagy at geoff@libsigil.com.
