<h1 align="center">
  <br>
  Minecraft Client Base
  <br>
</h1>

<h4 align="center">External client base for Minecraft using JNI.</h4>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#buildingusage">Building/Usage</a> •
  <a href="#crashes">Crashes</a> •
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
cd ../client
build.bat
```
4. Download the mappings for your desired version.
```
cd ../mappings
py download.py
```
5. Start Minecraft and run `build/inject.exe` to inject the loader. The loader will automatically load the client into the game.

## Crashes

If Minecraft crashes when the loader is injected, try the following:

* Injecting **two or more times** will cause the game to crash. This is not a bug, and is caused by the "client" not being detached properly, which redefines the classes.
* Run Minecraft with **logging enabled**. If it crashes without any logging, then there is most likely a problem with your JNI version. Set the full JNI header and library paths in the `CMakelists.txt` file.
* Make sure you are using the **correct mappings** for your version. For example, if you are using the latest version of 1.8.9, choose "1.8.9 stable 22" in the download script.
* The "client" must be built using the **same version of java** as the version running the game. You can check your javac version by running `javac -version`. If the version is different, you can change the version by specifying the full path to the javac executable in the `client/build.bat` file.

## License

This project is licensed under the [BSD 3 license](LICENSE).
