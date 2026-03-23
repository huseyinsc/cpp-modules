cmake_minimum_required(VERSION 3.28)
project(TerminalUtils VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# 1. LIBRARY TARGET
# ---------------------------------------------------------------------------
add_library(terminal_utils_lib STATIC
    src/terminal_utils.cpp
)

target_include_directories(terminal_utils_lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------------------------
# 2. EXECUTABLE (Example/CLI)
# ---------------------------------------------------------------------------
add_executable(terminal_utils_cli src/main.cpp)

target_link_libraries(terminal_utils_cli PRIVATE terminal_utils_lib)

set_target_properties(terminal_utils_cli PROPERTIES 
    OUTPUT_NAME "terminal_utils"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)