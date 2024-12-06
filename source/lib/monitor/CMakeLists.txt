set(target monitor)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/Monitor.h
            )
set(sources ${source_path}/Monitor.cpp
			)
add_library(${target} ${sources} ${headers})
target_link_libraries(${target} PUBLIC core)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
