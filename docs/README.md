# Alvere

Alvere is intended to be a single-library game engine which provides API for creating 3D and 2D games. It is currently in the very early stages of development.

There exists a [test application](https://github.com/george-mcdonagh/alvere_engine/tree/master/alvere/alvere_application) in the repo which is being used as a sandbox for the Alvere engine.

### Dependencies

The repo is missing the third party libary binaries as they are too large. They can be downloaded separately [here](https://drive.google.com/uc?id=1jVAdz4ckPYVXvqeiaK1tjmpgNbDu-rFz&export=download). Download the zip file, extract its contents, and place the extracted `lib` folder in `alvere/alvere/dependencies`. All necessary linking should already be setup in the project.

Currently the engine depends on the following third party libraries:

|     |     |
| --- | --- |
| **glad** | OpenGL function loading |
| **glfw3** | Context creation, window + system events |
| **Assimp** | 3D model file loading |
| **IrrXML** | *Assimp dependency* |
| **zlib** | *Assimp dependency* |
| **FreeType** | Font file loading |
| **STB** | stb_image for image file loading |
| **LuaPlus** | Lua-C++ binding |
