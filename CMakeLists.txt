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

option(MakePNG "Produce PNG files using stb_image_write" OFF)


# NOTE: setting the following option to "ON" will produce PNG files using stb_image_write
set(MakePNG OFF)


# Defining target and compile definitions

if(MakePNG)
    add_compile_definitions(QRCODE_MAKE_PNG)
endif()

file(GLOB Src CONFIGURE_DEPENDS "src/*.c")
add_executable(${PROJECT_NAME} ${Src})
if(MakePNG)
    target_sources(${PROJECT_NAME} PRIVATE "src/dep/stb_image_write.c")
endif()

# NOTE: installing will only work after automating config editing
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY src/utils DESTINATION bin)

# NOTE: wut is going on here?