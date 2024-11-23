set(target salesman)
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(sources ${source_path}/main.cpp ${source_path}/salesman_position.cpp)			
add_executable(${target} ${sources})
target_link_libraries(${target} PUBLIC sa)
set_target_properties( ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/targets/${target}"
)
