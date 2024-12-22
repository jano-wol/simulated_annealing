set(target graphics)
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}")
set(sources ${source_path}/main.cpp
            ${source_path}/InfoUI.cpp
            ${source_path}/MenuUI.cpp
            ${source_path}/SACallUI.cpp
            ${source_path}/SAFactoryUI.cpp
            ${source_path}/StateUI.cpp
            )		
set(headers ${source_path}/InfoUI.h
            ${source_path}/MenuUI.h
            ${source_path}/SACallUI.h
            ${source_path}/SAFactoryUI.h
            ${source_path}/StateUI.h
            )  
add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} PUBLIC core imgui implot io sa serializator)

