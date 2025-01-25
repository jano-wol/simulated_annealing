set(target minimal_spanning_tree)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/Generator.h
            ${include_path}/Move.h
            ${include_path}/Position.h
            )
set(sources ${source_path}/Generator.cpp
            ${source_path}/Move.cpp
            ${source_path}/Position.cpp
			)           
add_library(${target} ${sources} ${headers})
target_link_libraries(${target} PUBLIC core imgui implot serializator)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
