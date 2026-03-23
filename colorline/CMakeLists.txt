cmake_minimum_required(VERSION 3.28) # Modül desteği için 3.28+ önerilir
project(ColorLine VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# 1. LIBRARY TARGET
# ---------------------------------------------------------------------------
# creating a library that only contains the module and header.
add_library(colorline_lib)

target_sources(colorline_lib
    PUBLIC
        FILE_SET CXX_MODULES FILES src/colorline.cppm
)

target_include_directories(colorline_lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------------------------
# 2. EXECUTABLE (Example/CLI)
# ---------------------------------------------------------------------------
# This section compiles the program you will use from the terminal.
add_executable(colorline_cli src/main.cpp)

# Enable the application to use the library
target_link_libraries(colorline_cli PRIVATE colorline_lib)

set_target_properties(colorline_cli PROPERTIES 
    OUTPUT_NAME "colorline"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" # To place final binary into the "bin" folder (Optional)
)