cmake_minimum_required(VERSION 3.14)
project(
    "vlc-bittorrent"
    VERSION 2.16
    DESCRIPTION "A bittorrent plugin for VLC."
    HOMEPAGE_URL "https://github.com/johang/vlc-bittorrent"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)

if(WIN32)
    set(VLC_INSTALL_DIR "C:/Program Files/VideoLAN/VLC" CACHE PATH "VLC installation directory (runtime)")
    set(VLC_INCLUDE_DIR "C:/msys64/mingw64/include" CACHE PATH "VLC include directory (for building)")
    set(VLC_LIB_DIR "C:/msys64/mingw64/lib" CACHE PATH "VLC library directory (for building)")
    set(LIBTORRENT_INCLUDE_DIR "C:/msys64/mingw64/include" CACHE PATH "libtorrent include directory")
    set(LIBTORRENT_LIB_DIR "C:/msys64/mingw64/lib" CACHE PATH "libtorrent library directory")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(VLC_PLUGIN REQUIRED vlc-plugin>=3.0.0)
    pkg_check_modules(LIBTORRENT_RASTERBAR REQUIRED libtorrent-rasterbar>=2.0)
endif()

find_package(Threads REQUIRED)

add_subdirectory(src)