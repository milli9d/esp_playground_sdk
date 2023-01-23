#
# "drivers" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


COMPONENT_ADD_INCLUDEDIRS += dht/include inky/include 

COMPONENT_SRCDIRS += dht inky/src