set(target targets-salesman-tests)

set(sources
   TestSalesman.cpp
)

add_executable(${target} ${sources})

target_link_libraries(${target}
	PUBLIC
	monitor
	sa	
	salesman
	serializator
	PRIVATE
	gmock-dev
	GTest::gtest_main
)
enable_sanitizers(${target})

gtest_discover_tests(${target})

