set(target policies)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/Acceptance.h
            ${include_path}/Cooling.h
            ${include_path}/MoveSelector.h
            ${include_path}/Resource.h
            )
set(sources ${source_path}/Acceptance.cpp
            ${source_path}/Cooling.cpp
            ${source_path}/MoveSelector.cpp
            ${source_path}/Resource.cpp
			)           
add_library(${target} ${sources} ${headers})
target_link_libraries(${target} PUBLIC core)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
