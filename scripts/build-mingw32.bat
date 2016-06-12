:: a script for simplifying my life when building and distributing a new version of SIGIL;
:: in short, it invokes the build processes for all of my MinGW-based distributions and
:: then copies the resulting libs into the pre-compiled libraries folder and the examples,
:: and then finally, builds the examples
:: Geoff Nagy
:: ------------------------------------------------------------------------------

@echo off

:: simple alias to Codeblocks to reduce clutter below
set codeblocks="C:\Program Files (x86)\CodeBlocks\codeblocks.exe"

:: building SIGIL with MinGW is easier than with MSVC...
echo ------------------------------
echo building SIGIL with MinGW32...

:: force a rebuild of the entire SIGIL lib
cd ../build-mingw32
mingw32-make clean
mingw32-make
cd ../

:: copy our libraries into the pre-compiled SIGIL folders and example folders
call :copy_mingw_libraries mingw32

:: lastly, rebuild our examples for MinGW
call :build_mingw_examples mingw32

:: and we're done!
echo ----------------------------
echo done
exit /B %ERRORLEVEL%

:: ------------------------------------------------------------------------------
:: args: name of compiler
:copy_mingw_libraries
	call :copy_mingw_to_precomps %1
	call :copy_mingw_to_example %1 input
	call :copy_mingw_to_example %1 shapes
	call :copy_mingw_to_example %1 sound
	call :copy_mingw_to_example %1 sprites
	call :copy_mingw_to_example %1 text
	call :copy_mingw_to_example %1 transformations
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., mingw32)
:copy_mingw_to_precomps
	echo copying libsigil.dll and libsigil.dll.a to precompiled %1 lib folder...
	copy build-%1\libsigil.dll sigil-%1\libsigil.dll >nul
	copy build-%1\libsigil.dll.a sigil-%1\lib\libsigil.dll.a >nul
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., mingw32) and name of example (e.g., input)
:copy_mingw_to_example
	echo copying libsigil.dll and libsigil.dll.a to example %2 for %1...
	copy build-%1\libsigil.dll examples\%2\%1\libsigil.dll >nul
	copy build-%1\libsigil.dll.a examples\%2\%1\lib\libsigil.dll.a >nul
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., mingw32), and environment vars file
: build_mingw_examples
	call :build_mingw_example %1 input
	call :build_mingw_example %1 shapes
	call :build_mingw_example %1 sound
	call :build_mingw_example %1 sprites
	call :build_mingw_example %1 text
	call :build_mingw_example %1 transformations
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., mingw32), and example name
:build_mingw_example
	echo building example %2 for %1...
	%codeblocks% --no-splash-screen --rebuild --target="Release" examples/%2/%1/%2-%1.cbp
exit /B 0