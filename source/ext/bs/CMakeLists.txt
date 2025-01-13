set(target bs)
add_library(${target} INTERFACE)
# Disable external library warnings
set_property(TARGET bs PROPERTY COMPILE_OPTIONS)
target_include_directories(${target} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
