cmake_minimum_required(VERSION 3.16)
get_filename_component(PROJECT_ID ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_ID} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sätt Debug som standard om inget annat anges
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Kompileringsflaggor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Delad modell-kod
set(MODEL_SOURCES
    src/model/Board.cpp
    src/model/Game.cpp
)

# --- Terminal-version ---
add_executable(minesweeper_terminal
    src/minesweeper.cpp
    src/view/Renderer.cpp
    src/controller/InputHandler.cpp
    ${MODEL_SOURCES}
)
target_include_directories(minesweeper_terminal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- GUI-version (raylib) ---
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(minesweeper_gui
    src/gui/main.cpp
    src/gui/GuiRenderer.cpp
    ${MODEL_SOURCES}
)
target_include_directories(minesweeper_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(minesweeper_gui raylib)
