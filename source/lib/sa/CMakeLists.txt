set(target sa)
set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(headers ${include_path}/SA.h
            ${include_path}/SAFactory.h
            )
set(sources ${source_path}/SA.cpp
            ${source_path}/SAFactory.cpp
            )
add_library(${target} ${sources} ${headers})
target_link_libraries(${target} PUBLIC core monitor policies)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
