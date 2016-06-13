#!/bin/bash

# a script for simplifying my life when building and distributing a new version of SIGIL;
# in short, it invokes the build processes for all of my Linux-GCC-based distributions and
# then compiles the examples
# Geoff Nagy
# ------------------------------------------------------------------------------

# building SIGIL in Linux is easier than in Windows...
printf "==============================\n"
printf "building SIGIL with GCC...\n"

# force a rebuild of the entire SIGIL lib
cd ../build-linux-gcc
make clean
make
sudo make install
cd ../

# now compile our examples
codeblocks --no-splash-screen --rebuild --target="Release" examples/input/linux-gcc/input-linux-gcc.cbp
codeblocks --no-splash-screen --rebuild --target="Release" examples/shapes/linux-gcc/shapes-linux-gcc.cbp
codeblocks --no-splash-screen --rebuild --target="Release" examples/sound/linux-gcc/sound-linux-gcc.cbp
codeblocks --no-splash-screen --rebuild --target="Release" examples/sprites/linux-gcc/sprites-linux-gcc.cbp
codeblocks --no-splash-screen --rebuild --target="Release" examples/text/linux-gcc/text-linux-gcc.cbp
codeblocks --no-splash-screen --rebuild --target="Release" examples/transformations/linux-gcc/transformations-linux-gcc.cbp

# and we're done!
printf "==============================\n"
printf "done\n"

