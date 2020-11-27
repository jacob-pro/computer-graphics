#!/bin/bash

set -e
cmake.exe --configure . "-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake.exe --build . --config Release
rm -rf ./animation
mkdir "animation"
Release/CG2020.exe --animation
ffmpeg.exe -framerate 60 -i './animation/%d.bmp' -vf subtitles=subtitle.ass -c:v libx264 -y -hide_banner -loglevel panic out.mp4
VLC=$(wslpath "C:\Program Files\VideoLAN\VLC\vlc.exe")
"$VLC" out.mp4
