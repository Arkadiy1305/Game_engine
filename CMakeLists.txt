# CMakeList.txt: проект CMake для CMakeProject1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.28)

set(PROJECT_NAME "MY_PROJECT")
project (${PROJECT_NAME})

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    # this change happened in CMake 3.20.4
    set(CMAKE_CXX_STANDARD 23) # /std:c++latest - unlocks the non stable cpp20 features. For new 16.11 versions
else ()
    set(CMAKE_CXX_STANDARD 20) # /std:c++latest for msvc and -std=c++20 for everyone else.
endif ()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

set(OALPP_ENABLE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(OALPP_ENABLE_APPROVAL_TESTS OFF CACHE BOOL "" FORCE)
set(OALPP_ENABLE_INTEGRATION_TESTS OFF CACHE BOOL "" FORCE)


# Добавьте источник в исполняемый файл этого проекта.
add_executable (${PROJECT_NAME} "src/CMakeProject1.cpp" "src/CMakeProject1.h"  "src/ShaderClass.h" "src/VAO.h" "src/TextRender.h")

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(external/oalpp)
target_link_libraries(${PROJECT_NAME} OpenALpp_Lib)

add_subdirectory(external/freetype)
target_link_libraries(${PROJECT_NAME} freetype)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
