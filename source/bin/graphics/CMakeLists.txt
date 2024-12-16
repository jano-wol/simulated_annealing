set(target graphics)
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(sources ${source_path}/main.cpp
            ${source_path}/Menubar.cpp
            )		
set(headers ${source_path}/Menubar.h
            )  
add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} PUBLIC core imgui implot io sa serializator)

