cmake_minimum_required(VERSION 3.30)
project(CHIP8)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

# Set the SDL version
set(SDL_VERSION 3.2.8)
# If SDL is not found the download and compile it
if (NOT SDL3_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            SDL3
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL3-${SDL_VERSION}.zip
    )
    FetchContent_GetProperties(SDL3)
    if (NOT SDL_POPULATED)
        set(FETCHCONENT_QUIET NO)
        FetchContent_MakeAvailable(SDL3)
    endif()
endif()

# Add source files to the project
add_executable(${PROJECT_NAME} 
    ${PROJECT_NAME}/Main.cpp
    ${PROJECT_NAME}/Decode.cpp
    ${PROJECT_NAME}/Platform.cpp
    ${PROJECT_NAME}/Interpreter.cpp
)

# Link external libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
