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
2. Copy a magnet link to your clipboard
3. Go to **Media > Open Location from Clipboard** (or press `Ctrl+V` in the main window)
4. The magnet link will load automatically

Alternatively, you can use **Media > Open Network Stream** (or press `Ctrl+N`), paste the magnet link, and click **Play**.

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

### Build Environment
[MSYS2](https://www.msys2.org/) with MinGW-w64 64-bit toolchain. Open **MSYS2 MinGW 64-bit** terminal to build.

### Requirements (MSYS2 packages)

| Package | Version | Purpose | Linkage |
|---|---|---|---|
| `mingw-w64-x86_64-gcc` | ≥ 16.1.0 | C/C++ compiler | — |
| `mingw-w64-x86_64-cmake` | ≥ 4.3 | Build system (≥ 3.14) | — |
| `mingw-w64-x86_64-ninja` | ≥ 1.13 | Build runner | — |
| `mingw-w64-x86_64-vlc` | 3.0.x | VLC plugin API (`libvlc.dll.a`, `libvlccore.dll.a`) | dynamic (loaded by VLC) |
| `mingw-w64-x86_64-libtorrent-rasterbar` | ≥ 2.0 | P2P protocol engine | static |
| `mingw-w64-x86_64-openssl` | ≥ 3.0 | Encryption (libtorrent dependency) | static |
| `mingw-w64-x86_64-winpthreads` | ≥ 14.0 | pthreads (built from source → fully static `.a`) | static |

**Transitive dependencies** (auto-pulled by the above): `mingw-w64-x86_64-boost` (libtorrent dep), `mingw-w64-x86_64-gcc-libs`.

### Install Prerequisites

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
pacman -S mingw-w64-x86_64-vlc mingw-w64-x86_64-libtorrent-rasterbar mingw-w64-x86_64-openssl mingw-w64-x86_64-winpthreads
```

### Build Steps

```bash
# Clone
git clone https://github.com/omgbox/vlc-bittorrent.git
cd vlc-bittorrent

# Build
mkdir build && cd build
cmake .. -G Ninja
ninja
```

The output DLL will be at `build/src/libaccess_bittorrent_plugin.dll`.

### Static libwinpthreads (required for Windows VLC)

VLC 3.0 calls `SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32)`, which blocks plugins from loading `libwinpthread-1.dll` from MSYS2. The supplied `wp-static/libwinpthreads_static.a` is built from mingw-w64 source with `-DWINPTHREAD_STATIC` to eliminate this dependency. See `wp-static/README.md` for rebuild instructions.

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
