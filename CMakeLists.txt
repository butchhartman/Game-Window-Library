cmake_minimum_required(VERSION 3.21)
project(GWL
        VERSION 0.0.5
        DESCRIPTION "A windowing library"
        LANGUAGES C)

add_compile_definitions(GWL_VERSION="${PROJECT_VERSION}")

add_library(${PROJECT_NAME} STATIC)

if (MSVC AND PROJECT_IS_TOP_LEVEL)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Wall)
endif()

set_target_properties(${PROJECT_NAME} 
                        PROPERTIES
                        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib"
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/lib"
                        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
                        )

target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include)
target_include_directories(${PROJECT_NAME} PUBLIC include/glextensions)
target_link_libraries(${PROJECT_NAME} PUBLIC opengl32.lib)

add_subdirectory(src) 

enable_testing()
add_subdirectory(tests)