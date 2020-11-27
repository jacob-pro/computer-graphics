# Computer Graphics

Bristol COMS30020_2020 Computer Graphics (Y3), Coursework option

## Building

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install sdl2:x64-windows
```

Set `-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`

Ensure working directory is set so the obj/mtl files can be found.
