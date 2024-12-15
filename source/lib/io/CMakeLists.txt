set(target io)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/DirectoryIterator.h
            ${include_path}/Io.h
            )
set(sources ${source_path}/DirectoryIterator.cpp
            ${source_path}/Io.cpp
			)
add_library(${target} ${sources} ${headers})
target_link_libraries(${target} PUBLIC core serializator)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
