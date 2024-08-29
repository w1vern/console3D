cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set (PROJECT_NAME console3D)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
${CMAKE_SOURCE_DIR}/src/main.cpp
${CMAKE_SOURCE_DIR}/src/input.cpp 
${CMAKE_SOURCE_DIR}/include/input.h 
${CMAKE_SOURCE_DIR}/src/structures.cpp
${CMAKE_SOURCE_DIR}/include/structures.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(external/math_cpp)
target_link_libraries(${PROJECT_NAME} math_cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/math_cpp/include)

add_subdirectory(external/clean_console)
target_link_libraries(${PROJECT_NAME} clean_console)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/external/clean_console/include)