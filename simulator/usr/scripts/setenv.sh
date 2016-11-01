#!/bin/bash

SOCCERROOT=../..
export SOCCERROOT

PATH=$SOCCERROOT/usr/bin:$SOCCERROOT/usr/scripts:$PATH
export PATH

LD_LIBRARY_PATH=$SOCCERROOT/usr/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

LD_LIBRARY_PATH=$SOCCERROOT/usr/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

# Editor
export EDITOR=gedit

#used to be able to compile with global vars
LIBPATH=$SOCCERROOT/usr/lib
export LIBPATH

INCLUDEKOGMO=$SOCCERROOT/usr/include/rtdb
export INCLUDEKOGMO

INCLUDEROBOCTRL=$SOCCERROOT/usr/include/roboctrl
export INCLUDEROBOCTRL

INCLUDEMATH=$SOCCERROOT/usr/include/libmath
export INCLUDEMATH

# To surpress double includes
SOCCERLABconfigured=yes
export SOCCERLABconfigured

