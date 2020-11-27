# Computer Graphics

Bristol COMS30020_2020 Computer Graphics (Y3), Coursework option

## Features

- OBJ geometry file loading
- OBJ material file loading

- Wireframe rendering
- Triangle rasterising
- Perspective corrected texture mapping for rasteriser

- Keyboard control of camera position
- Changing camera orientation (using orientation matrix)
- Camera LookAt

- Diffuse lighting (proximity and angle-of-incidence)
- Specular lighting
- Hard Shadow with Ambient lighting
- Gouraud shading
- Phong Shading
- Soft shadows
- Reflective Materials - A Mirror

## Building on Windows

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install sdl2:x64-windows
```

Set `-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`

Ensure working directory is set so the obj/mtl files can be found.
