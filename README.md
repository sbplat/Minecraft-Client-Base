<h1 align="center">
  <br>
  Minecraft Client Base
  <br>
</h1>

<h4 align="center">External client base for Minecraft using JNI.</h4>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#buildingusage">Building/Usage</a> •
  <a href="#license">License</a>
</p>

## Features

* External injector and loader
* Currently supports vanilla and forge
* Open source

## Building/Usage

1. Clone this project.
2. Build the loader and injector using CMake.
```
mkdir build && cd build
cmake ..
cmake --build .
```
3. Build the client using the batch file.
```
cd client
build.bat
```
4. Start Minecraft and run `build/inject.exe` to inject the loader. The loader will automatically load the client into the game.

## License

This project is licensed under the [BSD 3 license](LICENSE).
