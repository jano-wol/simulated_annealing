set(target core)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/IMove.h
            ${include_path}/IPosition.h
            ${include_path}/Random.h
            ${include_path}/Statistics.h
            )
set(sources ${source_path}/IMove.cpp
            ${source_path}/IPosition.cpp
            ${source_path}/Random.cpp
            ${source_path}/Statistics.cpp
			)
add_library(${target} ${sources} ${headers})
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
