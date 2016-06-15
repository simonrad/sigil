:: a script for simplifying my life when building and distributing a new version of SIGIL;
:: in short, it invokes the build processes for all of my MSVC-based distributions and
:: then copies the resulting libs into the pre-compiled libraries folders and the examples,
:: and then finally, builds the examples
:: Geoff Nagy
:: ------------------------------------------------------------------------------

@echo off

:: build SIGIL with every MSVC compiler that we support
::call :build_msvc_sigil vc2010 "%VS100COMNTOOLS%vsvars32.bat"
call :build_msvc_sigil vc2010-win64 "%VS100COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2012 "%VS110COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2012-win64 "%VS110COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2013 "%VS120COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2013-win64 "%VS120COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2015 "%VS140COMNTOOLS%vsvars32.bat"
::call :build_msvc_sigil vc2015-win64 "%VS140COMNTOOLS%vsvars32.bat"

:: and we're done!
echo ----------------------------
echo done
exit /B %ERRORLEVEL%

:: ------------------------------------------------------------------------------
:: args: name of compiler (e.g., vc2010), and environment vars file
:build_msvc_sigil

	:: print some helpful message
	echo ------------------------------
	echo building SIGIL with %1...

	:: force a rebuild of the entire SIGIL lib
	call %2 >nul
	msbuild.exe ../build-%1/sigil.sln /t:Rebuild /property:Configuration=Release

	:: copy our libraries into the pre-compiled SIGIL folders and examples folders
	call :copy_msvc_libraries %1

	:: lastly, rebuild our examples for this compiler
	call :build_msvc_examples %1 %2

exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of compiler (e.g., vc2010)
:copy_msvc_libraries
	call :copy_msvc_to_precomps %1
	call :copy_msvc_to_example %1 input
	call :copy_msvc_to_example %1 shapes
	call :copy_msvc_to_example %1 sound
	call :copy_msvc_to_example %1 sprites
	call :copy_msvc_to_example %1 text
	call :copy_msvc_to_example %1 transformations
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., vc2010)
:copy_msvc_to_precomps
	echo copying sigil.dll and sigil.lib to precompiled %1 lib folder...
	copy ..\build-%1\Release\sigil.dll ..\sigil-%1\sigil.dll >nul
	copy ..\build-%1\Release\sigil.lib ..\sigil-%1\lib\sigil.lib >nul
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., vc2010) and name of example (e.g., input)
:copy_msvc_to_example
	echo copying sigil.dll and sigil.lib to example %2 for %1...
	copy ..\build-%1\Release\sigil.dll ..\examples\%2\%1\sigil.dll >nul
	copy ..\build-%1\Release\sigil.lib ..\examples\%2\%1\lib\sigil.lib >nul
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., vc2010), and environment vars file
: build_msvc_examples
	call :build_msvc_example %1 %2 input
	call :build_msvc_example %1 %2 shapes
	call :build_msvc_example %1 %2 sound
	call :build_msvc_example %1 %2 sprites
	call :build_msvc_example %1 %2 text
	call :build_msvc_example %1 %2 transformations
exit /B 0

:: ------------------------------------------------------------------------------
:: args: name of build (e.g., vc2010), environment vars file, and example name
:build_msvc_example
	echo building example %3 for %1...
	msbuild.exe ../examples/%3/%1/%3-%1.sln /t:Rebuild /property:Configuration=Debug
exit /B 0