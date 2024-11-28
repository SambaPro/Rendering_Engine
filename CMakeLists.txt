cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (RenderingEngine VERSION 3.8)

add_subdirectory(lib/imgui)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)

find_package(assimp CONFIG REQUIRED)


# Add source to this project's executable.
add_executable (${PROJECT_NAME} "src/main.cpp")


target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib/imgui")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib/glad")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib/glm")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib/stb_image")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/lib/glfw")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

