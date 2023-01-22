#
# "wrappers" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS += gpio_base/include

COMPONENT_SRCDIRS += gpio_base 

include $(PROJECT_PATH)/components/wrappers/gpio_base/component.mk