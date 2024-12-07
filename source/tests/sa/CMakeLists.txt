set(target sa-tests)

set(sources
   TestSa.cpp
)

add_executable(${target} ${sources})

target_link_libraries(${target}
	PUBLIC
	monitor
	sa
	PRIVATE
	gmock-dev
	GTest::gtest_main
)
enable_sanitizers(${target})

gtest_discover_tests(${target})

