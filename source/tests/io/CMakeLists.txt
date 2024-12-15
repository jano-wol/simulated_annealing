set(target io-tests)

set(sources
   TestIo.cpp
)

add_executable(${target} ${sources})

target_link_libraries(${target}
	PUBLIC
	io
	PRIVATE
	gmock-dev
	GTest::gtest_main
)
enable_sanitizers(${target})

gtest_discover_tests(${target})

