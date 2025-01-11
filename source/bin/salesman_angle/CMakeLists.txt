set(target salesman_angle-bin)
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(sources ${source_path}/main.cpp)			
add_executable(${target} ${sources})
target_link_libraries(${target} PUBLIC core io monitor sa targets)
set_target_properties( ${target}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${target}"
)
