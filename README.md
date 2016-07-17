# Sound, Input, and Graphics Integration Library (SIGIL)

A very simple cross-platform library for playing audio, handling basic input, and drawing 2D graphics such as sprites, shapes, and text. Designed to have little or no learning curve.

## Folder Structure:

**deps**: contains the CMake-based dependencies of SIGIL; some of these (GLFW and Freetype2) must be downloaded with Git depending on your build, but PIGU (for the Raspberry Pi) has been modified to use a CMake-based build system by myself and does not require this

**examples**: contains stand-alone example projects, with project files/makefiles for every single platform/compiler that SIGIL supports

**glew**: contains headers and pre-compiled Windows libs for the OpenGL Extension Wrangler library, which the Windows version of SIGIL depends on

**include**: contains the *sl.h* header file you need to include in your applications to use SIGIL

**scripts**: contains scripts designed to simplify the mass-build process I use for SIGIL

**sigil-XXXXXX**: contains DLLs and static libs for SIGIL built using various compilers

**src**: SIGIL source code, as well as some source code from some open-source libraries that SIGIL depends on (and are not CMake-based)

## Building from Source

Build instructions are located in this directory with the name *BUILD-XXXXXX.txt* for every platform/compiler that SIGIL supports.

## License Information

View *LICENSE.txt* for license information.

## Contact

Questions, comments, and bug reports should be directed towards geoff@libsigil.com.
