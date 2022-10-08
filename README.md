<h1 align="center">
  <br>
  Minecraft Client Base
  <br>
</h1>

<h4 align="center">External client base for Minecraft using JNI.</h4>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#buildingusage">Building/Usage</a> •
  <a href="#mappings">Mappings</a> •
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

## Mappings

The included mappings are for Minecraft v1.8.9. For other versions, overwrite the included mappings with the mappings from your desired version. Mappings can be obtained from [here](https://web.archive.org/web/20211108233611/http://export.mcpbot.bspk.rs/).

Example for 1.8.9 mappings:
* [fields, methods, params](https://web.archive.org/web/20210925224641if_/http://export.mcpbot.bspk.rs/mcp_stable/18-1.8/mcp_stable-18-1.8.zip)
* [joined, static_methods](https://web.archive.org/web/20210925224724if_/http://export.mcpbot.bspk.rs/mcp/1.8.9/mcp-1.8.9-srg.zip)

## License

This project is licensed under the [BSD 3 license](LICENSE).
