cmake_minimum_required(VERSION 3.28) # Modül desteği için 3.28+ önerilir
project(ColorLine VERSION 1.0.0 LANGUAGES CXX)

# C++ Standart Ayarı
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# 1. KÜTÜPHANE HEDEFİ (Library)
# ---------------------------------------------------------------------------
# Sadece modül ve header'ı içeren bir kütüphane oluşturuyoruz.
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
# 2. UYGULAMA HEDEFİ (Executable/CLI)
# ---------------------------------------------------------------------------
# Bu kısım senin terminalden kullanacağın programı derler.
add_executable(colorline_cli src/main.cpp)

# Uygulamanın kütüphaneyi kullanmasını sağla
target_link_libraries(colorline_cli PRIVATE colorline_lib)

# Çıktı ismini senin istediğin gibi "colorline" yapalım
set_target_properties(colorline_cli PROPERTIES OUTPUT_NAME "colorline")

# Nihai binary dosyasını "bin" klasörüne atması için (Opsiyonel)
set_target_properties(colorline_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
