cmake_minimum_required(VERSION 3.16)

project(olc_gamejam_2025 LANGUAGES CXX)

# ----------------------
# Project Configuration
# ----------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------
# Source Files
# ----------------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/olcPixelGameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/olcPGEX_SplashScreen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/external/olcSoundWaveEngine.cpp
)

# ----------------------
# Include Paths
# ----------------------
# Prefer target_include_directories over global include_directories
# for better encapsulation.

add_executable(game ${GAME_SOURCES})

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/game/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/
)

# ----------------------
# Linking
# ----------------------
target_link_libraries(game)
