# Static libwinpthreads

This directory contains a fully static build of `libwinpthread` from the mingw-w64 source.

## Why

VLC 3.0 calls `SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_SYSTEM32)` at startup, which restricts DLL loading to `System32` only. The MSYS2 distribution's `libwinpthread-1.dll` cannot be loaded by VLC plugins, causing error 126.

## How to rebuild

```bash
# Clone mingw-w64 source
git clone https://git.code.sf.net/p/mingw-w64/mingw-w64 mingw-w64-src

# Build static libwinpthreads
cd mingw-w64-src/mingw-w64-libraries/winpthreads
mkdir build && cd build
../configure --host=x86_64-w64-mingw32 \
  --enable-static --disable-shared \
  CFLAGS="-DWINPTHREAD_STATIC -O2"
make -j$(nproc)

# Copy the static library
cp .libs/libwinpthread.a /path/to/project/wp-static/libwinpthreads_static.a
```

## Files

- `libwinpthreads_static.a` - Static library
- `*.o` - Individual object files (for reference)
