# ENG2D
2d c++ game engine built on SDL3 and Box2D

includes SDL3's satellite libraries (image, ttf, mixer, etc...) and nlohmann's json library as well

## CMake Build Process

"generator": "MinGW Makefiles",


### you'll need the following libs installed:

pacman -S mingw-w64-x86_64-gcc

pacman -S mingw-w64-x86_64-gdb

pacman -S mingw-w64-x86_64-cmake

pacman -S mingw-w64-x86_64-freetype

### And Here's an Example CMakeLists.txt for a project called "game":

    cmake_minimum_required(VERSION 3.20)

    project(game LANGUAGES CXX)

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for vscode intellisense

    include(FetchContent)
    FetchContent_Declare(
        ENG2D
        GIT_REPOSITORY https://github.com/shadowcrafter01/ENG2D.git
        GIT_TAG main
    )
    set(ENG2D_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(ENG2D)

    add_executable(
        game
        src/main.cpp
    )

    target_link_libraries(
        game
        PRIVATE
            ENG2D::ENG2D
    )

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

    if(WIN32)

        add_custom_command(
            TARGET game
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_image::SDL3_image>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
                "$<TARGET_FILE_DIR:game>"
        )

    endif()

## Command-Line building:

This isnt really an intended way to build this thing, but you could just grab everything in include/ and use it in a project you already have with all the dependencies working

i might come up with a more official way to do so later on, but it just works so much cleaner with CMake