set(target policies)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/IResourcePolicy.h
            ${include_path}/IterationPolicy.h
            )
set(sources ${source_path}/IterationPolicy.cpp
			)           
add_library(${target} ${sources} ${headers})
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
