set(target graphics)
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(sources ${source_path}/main.cpp
            )		
set(headers )	  
add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} PUBLIC core imgui io sa serializator)

