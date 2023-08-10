# Immersive Activate

A fork from [po3 Simple Activate SKSE](https://github.com/powerof3/SimpleActivateSKSE). Additionally it replaces the name of the object pointed by the crosshair with generic names based on the object form type.

## Requirements
* [CMake](https://cmake.org/)
* [Vcpkg](https://github.com/microsoft/vcpkg)
* [Visual Studio Community Preview 2022](https://visualstudio.microsoft.com/): Install desktop development with C++ extension
* [CommonLibSSE](https://github.com/dann1/CommonLibSSE/tree/dev): This is already included as git submodle
* **Environmental Variables**:
  * Add the cmake executable to your `PATH` environmental variable
  * Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
  * Optionally set the `SkyrimModsPath` environment variable to the Mod staging folder. If set, when building the project, the plugin will be copied to `$SkyrimModsPath/ImmersiveActivate`



## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```pwsh
git clone https://github.com/dann1/ImmersiveActivate.git
cd ImmersiveActivate
cmake -B build -S .
```
