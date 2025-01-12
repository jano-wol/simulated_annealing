set(target imgui)
find_package(glfw3 REQUIRED)
if(NOT glfw3_FOUND)
  message(FATAL_ERROR "GLFW not found. Please install GLFW.")
endif()
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
  message(FATAL_ERROR "OpenGL not found.")
endif()
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(headers ${source_path}/imgui/imconfig.h
            ${source_path}/imgui/imgui.h
            ${source_path}/imgui/imgui_impl_glfw.h
            ${source_path}/imgui/imgui_impl_opengl3.h
            ${source_path}/imgui/imgui_impl_opengl3_loader.h
            ${source_path}/imgui/imgui_internal.h
            ${source_path}/imgui/imstb_rectpack.h
            ${source_path}/imgui/imstb_textedit.h
            ${source_path}/imgui/imstb_truetype.h
)
set(sources ${source_path}/imgui/imgui.cpp
            ${source_path}/imgui/imgui_demo.cpp
            ${source_path}/imgui/imgui_draw.cpp
            ${source_path}/imgui/imgui_impl_glfw.cpp
            ${source_path}/imgui/imgui_impl_opengl3.cpp
            ${source_path}/imgui/imgui_tables.cpp
            ${source_path}/imgui/imgui_widgets.cpp
)
add_library(${target} ${headers} ${sources})
# Disable external library warnings
set_property(TARGET imgui PROPERTY COMPILE_OPTIONS)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
target_link_libraries(${target} glfw OpenGL::GL)
