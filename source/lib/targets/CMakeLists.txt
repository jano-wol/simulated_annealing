# Define the umbrella library "targets" as an INTERFACE library
add_library(targets INTERFACE)

# Collect and link the subtargets dynamically
file(GLOB SUBTARGETS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(subdir ${SUBTARGETS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
        add_subdirectory(${subdir})
        # Link subtarget to "targets" (automatically propagates its properties)
        target_link_libraries(targets INTERFACE ${subdir})
        # Collect include directories and add them to the umbrella target
        target_include_directories(targets INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/include)
    endif()
endforeach()

# Add the central Targets.h to the include directories
target_include_directories(targets INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})