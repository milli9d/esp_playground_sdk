file(GLOB app_srcs ${CMAKE_CURRENT_SOURCE_DIR}/.*cpp)

idf_component_register(SRCS ${app_srcs}
                    INCLUDE_DIRS ".")