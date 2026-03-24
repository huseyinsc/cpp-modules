cmake_minimum_required(VERSION 3.28)
project(TerminalUtils VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================== Bağımlılık Çözümleme: Colorline (Header-Only) ====================
set(COLORLINE_INCLUDE_DIR "")

# Global Cache Dizinini Belirle
if(WIN32)
    set(DEFAULT_CACHE_DIR "$ENV{LOCALAPPDATA}/cpp-modules")
else()
    set(DEFAULT_CACHE_DIR "$ENV{HOME}/.cache/cpp-modules")
endif()
set(FETCHCONTENT_BASE_DIR "${DEFAULT_CACHE_DIR}" CACHE PATH "Global shared cache" FORCE)

# 1. Üst Dizinde Var Mı? (Monorepo kullanımı)
set(UPPER_DIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../colorline")
if(EXISTS "${UPPER_DIR_PATH}/include/colorline.hpp")
    message(STATUS "[TerminalUtils] Colorline (Header) üst dizinden bağlandı: ${UPPER_DIR_PATH}")
    set(COLORLINE_INCLUDE_DIR "${UPPER_DIR_PATH}/include")
endif()

# 2. Global Cache'de Var Mı? 
if(NOT COLORLINE_INCLUDE_DIR)
    set(CACHE_DIR_PATH "${DEFAULT_CACHE_DIR}/cpp_modules-src/public/colorline")
    if(EXISTS "${CACHE_DIR_PATH}/include/colorline.hpp")
        message(STATUS "[TerminalUtils] Colorline (Header) global cache'den bağlandı: ${CACHE_DIR_PATH}")
        set(COLORLINE_INCLUDE_DIR "${CACHE_DIR_PATH}/include")
    endif()
endif()

# 3. İkisinde de yoksa FetchContent ile indir ve SADECE Include yolunu al
if(NOT COLORLINE_INCLUDE_DIR)
    message(STATUS "[TerminalUtils] Colorline bulunamadı. GitHub'dan indiriliyor...")
    include(FetchContent)
    FetchContent_Declare(
        cpp_modules
        GIT_REPOSITORY https://github.com/huseyinsc/cpp-modules.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
    )
    # Sadece kaynak kodları indir, alt hedefleri (add_subdirectory) çalıştırma
    FetchContent_Populate(cpp_modules) 
    set(COLORLINE_INCLUDE_DIR "${cpp_modules_SOURCE_DIR}/public/colorline/include")
    message(STATUS "[TerminalUtils] Colorline (Header) indirildi ve bağlandı.")
endif()

# ---------------------------------------------------------------------------
# 1. LIBRARY TARGET
# ---------------------------------------------------------------------------
add_library(terminal_utils_lib STATIC
    src/terminal_utils.cpp
)

# Kendi include dizinimizi VE colorline'ın include dizinini kütüphaneye tanıtıyoruz
target_include_directories(terminal_utils_lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${COLORLINE_INCLUDE_DIR}
)

# ---------------------------------------------------------------------------
# 2. EXECUTABLE (Example/CLI)
# ---------------------------------------------------------------------------
add_executable(terminal_utils_cli src/main.cpp)

# Uygulamamıza sadece terminal_utils_lib'i bağlıyoruz. 
# (Colorline header-only olduğu için ayrıca linklemeye gerek yok)
target_link_libraries(terminal_utils_cli PRIVATE terminal_utils_lib)

set_target_properties(terminal_utils_cli PROPERTIES 
    OUTPUT_NAME "terminal_utils"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)