set(target implot)
find_package(glfw3 REQUIRED)
if(NOT glfw3_FOUND)
  message(FATAL_ERROR "GLFW not found. Please install GLFW.")
endif()
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
  message(FATAL_ERROR "OpenGL not found.")
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
