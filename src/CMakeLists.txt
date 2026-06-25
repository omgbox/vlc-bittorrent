add_library(
    access_bittorrent_plugin
    MODULE
    module.cpp
    metadata.cpp
    magnetmetadata.cpp
    data.cpp
    download.cpp
    session.cpp
    vlc.cpp
)

if(WIN32)
    target_include_directories(
        access_bittorrent_plugin
        PRIVATE
        "${LIBTORRENT_INCLUDE_DIR}"
        "${VLC_INCLUDE_DIR}"
    )

    target_link_directories(
        access_bittorrent_plugin
        PRIVATE
        "${LIBTORRENT_LIB_DIR}"
        "${VLC_LIB_DIR}"
    )

    target_link_libraries(
        access_bittorrent_plugin
        PRIVATE
        "-lvlc"
        "-lvlccore"
        "-Wl,--whole-archive"
        "${CMAKE_SOURCE_DIR}/wp-static/libwinpthreads_static.a"
        "-Wl,--no-whole-archive"
        "${LIBTORRENT_LIB_DIR}/libtorrent-rasterbar.a"
        "${LIBTORRENT_LIB_DIR}/libssl.a"
        "${LIBTORRENT_LIB_DIR}/libcrypto.a"
        "-lws2_32"
        "-liphlpapi"
        "-lcrypt32"
        "-lbcrypt"
        "-lmswsock"
    )

    target_link_options(
        access_bittorrent_plugin
        PRIVATE
        -static-libgcc
        -static-libstdc++
    )

    target_compile_options(
        access_bittorrent_plugin
        PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-deprecated
        -D_GNU_SOURCE
    )
else()
    target_include_directories(
        access_bittorrent_plugin
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${VLC_PLUGIN_INCLUDE_DIRS}
        ${LIBTORRENT_RASTERBAR_INCLUDE_DIRS}
    )

    target_link_libraries(
        access_bittorrent_plugin
        PRIVATE
        ${VLC_PLUGIN_LINK_LIBRARIES}
        ${LIBTORRENT_RASTERBAR_LINK_LIBRARIES}
        Threads::Threads
    )
endif()

target_compile_definitions(
    access_bittorrent_plugin
    PRIVATE
    -D__PLUGIN__
    -DMODULE_STRING="bittorrent"
    -DPACKAGE="vlc-bittorrent"
)

set_target_properties(
    access_bittorrent_plugin
    PROPERTIES
    PREFIX "lib"
    OUTPUT_NAME "access_bittorrent_plugin"
    SUFFIX ".dll"
)