# GCSAudioControl
Audio Control application with presets

## Installation
### Dependencies
0. OC Microsoft Windows 10
1. C++ 17 Compiler
2. CMake

### Compiling
```
git clone https://github.com/Gaymocoder/GCSAudioControl.git
cd GCSAudioControl
mkdir build && cd build
cmake ..
cmake --build .
```
Executable file will be placed in "GCSAudioControl/bin" directory

### Creating file-preset (up to v0.1.0)
1. Create text file
2. Write to file in 2 lines volume level and audio device name, for example:
```
20
Динамики
```
3. Rename file to "preset.gcsap"
4. Open file with "GCSAudioControl.exe"
5. Preset has been applied (volume of device "Динамики" was set to 20%)

Examples for preset files can be found in examples directory