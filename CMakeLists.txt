cmake_minimum_required(VERSION 3.20.0)
project(
    ENG2D
    VERSION 0.1.0
    LANGUAGES C CXX
    )

include(FetchContent)

# include SDL3 and its satellites
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    IMG
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
#FetchContent_Declare(
#    TTF
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#    GIT_TAG main
#    GIT_SHALLOW TRUE
#)
FetchContent_Declare(
    MIXER
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
#FetchContent_Declare(
#    NET
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
#    GIT_TAG main
#    GIT_SHALLOW TRUE
#)
# include box2d
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.1.1
    GIT_SHALLOW TRUE
)
set(BOX2D_SAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 IMG MIXER box2d)

add_library(ENG2D INTERFACE)
add_library(ENG2D::ENG2D ALIAS ENG2D)

target_compile_features(
    ENG2D
    INTERFACE
    cxx_std_20
)

target_include_directories(
    ENG2D
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ENG2D
    INTERFACE
        SDL3::SDL3
        box2d
)