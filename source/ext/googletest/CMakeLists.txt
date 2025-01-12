set(gmock_build_tests             OFF CACHE BOOL "")
set(gtest_build_samples           OFF CACHE BOOL "")
set(gtest_build_tests             OFF CACHE BOOL "")
set(gtest_disable_pthreads        OFF CACHE BOOL "")
set(gtest_force_shared_crt        ON  CACHE BOOL "")
set(gtest_hide_internal_symbols   OFF CACHE BOOL "")

add_subdirectory(googletest-release-1.12.1)
set_target_properties(gtest PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)
set_target_properties(gtest_main PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)
set_target_properties(gmock PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)
set_target_properties(gmock_main PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)

add_library(gmock-dev INTERFACE)

# Disable external library warnings
set_property(TARGET gtest PROPERTY COMPILE_OPTIONS)
set_property(TARGET gtest_main PROPERTY COMPILE_OPTIONS)
set_property(TARGET gmock PROPERTY COMPILE_OPTIONS)
set_property(TARGET gmock_main PROPERTY COMPILE_OPTIONS)

target_include_directories(gmock-dev
		SYSTEM INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/googletest-release-1.12.1/googletest/include
		${CMAKE_CURRENT_SOURCE_DIR}/googletest-release-1.12.1/googlemock/include
		)
		
target_link_libraries(gmock-dev
		INTERFACE
		gmock
)		
