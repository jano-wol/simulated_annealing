set(target implot)
find_library(glfw_lib glfw)
if(NOT glfw_lib)
  message(FATAL_ERROR "glfw not found")
endif()
find_library(gl_lib GL)
if(NOT gl_lib)
  message(FATAL_ERROR "gl not found")
endif()
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(headers ${source_path}/implot/implot.h
            ${source_path}/implot/implot_internal.h
            )
set(sources ${source_path}/implot/implot.cpp
            ${source_path}/implot/implot_items.cpp
            ${source_path}/implot/implot_demo.cpp
            )
add_library(${target} ${headers} ${sources})
# Disable external library warnings
set_property(TARGET implot PROPERTY COMPILE_OPTIONS)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${target} glfw GL imgui)
