#
# "components" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS += drivers/dht/include wrappers/gpio_base/include utils/smart_timer/include

COMPONENT_SRCDIRS += drivers/dht wrappers/gpio_base utils/smart_timer

# COMPONENT_DIRS += drivers wrappers utils


# include $(PROJECT_PATH)/components/drivers/component.mk
# include $(PROJECT_PATH)/components/wrappers/component.mk
# include $(PROJECT_PATH)/components/utils/component.mk
