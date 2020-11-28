#!/bin/bash

# This script ensures that the game executable is run from the correct working directory,
# so that the relative asset paths expected by the game work.

EXE=build/chalice_chaser
SCRIPT_DIR=`dirname $0`

cd $SCRIPT_DIR
if [ -x $EXE ]
  then
    $EXE
else
  echo "Game executable not found at $SCRIPT_DIR/$EXE"
  echo "See README for build instructions"
fi
