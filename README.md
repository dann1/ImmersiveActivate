# Immersive Activate

A fork from [po3 Simple Activate SKSE](https://github.com/powerof3/SimpleActivateSKSE). Additionally it replaces the name of the object pointed by the crosshair with generic names based on the object form type.

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community Preview 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSSE](https://github.com/dann1/CommonLibSSE/tree/dev)
	* You need to build from the dann1/dev branch
	* Add this as as an environment variable `CommonLibSSEPath`
* Mod Manager: Optionally set the `SkyrimModsPath` environment variable to the Mod staging folder. If set when building the project, the DLL and the PDB files will be copied to `$SkyrimModsPath/ImmersiveActivate`.

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/dann1/ImmersiveActivate.git
cd ImmersiveActivate
cmake -B build -S .
```
