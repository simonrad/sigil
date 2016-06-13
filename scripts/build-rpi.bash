#!/bin/bash

# a script for simplifying my life when building and distributing a new version of SIGIL;
# in short, it invokes the build processes for all of my Raspberry Pi-based distributions
# and then compiles the examples
# Geoff Nagy
# ------------------------------------------------------------------------------

# building SIGIL in Linux is easier than in Windows...
printf "==============================\n"
printf "building SIGIL with GCC Make for RPi...\n"

# force a rebuild of the entire SIGIL lib
cd ../build-rpi
make clean
make
sudo make install
cd ../

# now compile our examples
printf "==============================\n"
printf "building examples for RPi...\n"
make --directory=examples/input/rpi
make --directory=examples/shapes/rpi
make --directory=examples/sound/rpi
make --directory=examples/sprites/rpi
make --directory=examples/text/rpi
make --directory=examples/transformations/rpi

# and we're done!
printf "==============================\n"
printf "done\n"

