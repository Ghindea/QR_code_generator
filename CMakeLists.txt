cmake_minimum_required(VERSION 3.20)

project(QR_code LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

file(GLOB_RECURSE Src "src/*.c")
add_executable(${PROJECT_NAME} ${Src})

# NOTE: installing will only work after automating config editing
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY src/utils DESTINATION bin)

# NOTE: wut is going on here?