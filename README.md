# vlc-bittorrent

A VLC media player plugin that adds support for streaming torrents and magnet links directly inside VLC. Based on [johang/vlc-bittorrent](https://github.com/johang/vlc-bittorrent) ([PR #53](https://github.com/johang/vlc-bittorrent/pull/53)), rebuilt from source for Windows with fully static linking.

## Features

- Open `.torrent` files directly in VLC
- Stream from `magnet:` links by pasting them into VLC
- Bittorrent data access, metadata access, and magnet metadata access
- Zero external DLL dependencies -- the plugin is a single self-contained file

## Download

Download `libaccess_bittorrent_plugin.dll` from the [Releases](https://github.com/omgbox/vlc-bittorrent/releases) page.

## Installation

1. Install [VLC media player](https://www.videolan.org/) (3.0.x, 64-bit)
2. Copy `libaccess_bittorrent_plugin.dll` into your VLC plugins directory:
   ```
   C:\Program Files\VideoLAN\VLC\plugins\access\
   ```
3. Restart VLC

## Usage

### Magnet Links
1. Open VLC
2. Go to **Media > Open Location from Clipboard** 
3. Paste a magnet link (e.g. `magnet:?xt=urn:btih:...`)
4. Click **Play**

### Torrent Files
1. Open VLC
2. Go to **Media > Open File** (or press `Ctrl+O`)
3. Select a `.torrent` file
4. Click **Open**

### Command Line
```bat
vlc "magnet:?xt=urn:btih:EXAMPLE&dn=Example+Torrent"
vlc "C:\path\to\file.torrent"
```

## Building from Source

### Requirements
- [MSYS2](https://www.msys2.org/) with MinGW 64-bit toolchain
- VLC 3.0.x development headers and libraries (via MSYS2: `pacman -S mingw-w64-x86_64-vlc`)
- libtorrent-rasterbar (via MSYS2: `pacman -S mingw-w64-x86_64-libtorrent-rasterbar`)
- OpenSSL (via MSYS2: `pacman -S mingw-w64-x86_64-openssl`)
- Ninja (`pacman -S mingw-w64-x86_64-ninja`)

### Build Steps

Open **MSYS2 MinGW 64-bit** terminal:

```bash
# Clone the repository
git clone https://github.com/omgbox/vlc-bittorrent.git
cd vlc-bittorrent

# Build
mkdir build && cd build
cmake .. -G Ninja
ninja
```

The output DLL will be at `build/src/libaccess_bittorrent_plugin.dll`.

### Deploy to MSYS2 VLC (for testing)
```bash
cp build/src/libaccess_bittorrent_plugin.dll /mingw64/lib/vlc/plugins/access/
```

## Technical Details

- **Compiler:** GCC 16.1.0 (MSYS2 MinGW-w64)
- **Static linking:** libgcc, libstdc++, libwinpthread, libtorrent-rasterbar, OpenSSL, libcrypto
- **Windows VLC compatibility:** Uses a fully static winpthreads built from mingw-w64 source to avoid the `libwinpthread-1.dll` dependency, which is required because VLC 3.0 calls `SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32)` restricting plugin DLL loading to system DLLs only
- **Supported VLC version:** 3.0.x (Vetinari), built with MinGW-w64 GCC

## License

This project is based on [vlc-bittorrent](https://github.com/johang/vlc-bittorrent) by Johan Gunnarsson, which is licensed under the GNU General Public License v2. This project inherits the same license.

## Credits

- [johang/vlc-bittorrent](https://github.com/johang/vlc-bittorrent) - Original plugin
- [PR #53](https://github.com/johang/vlc-bittorrent/pull/53) - Windows support patches
- [mingw-w64 winpthreads](https://sourceforge.net/projects/mingw-w64/) - Static pthreads implementation for Windows
