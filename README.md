# LearnOpenGL Examples with SDL and bgfx

This repository contains examples from LearnOpenGL implemented using SDL and bgfx. Works currently on macOS, Linux and Windows.
Planning to add more examples from LearnOpenGL and more platforms support.


## Clone the repo

```sh
git clone https://github.com/seemsindie/learnbgfx.git
cd learnopengl-sdl-bgfx
```

## Building the project

Note: on Windows install nasm (SDL image requierment) with `winget install nasm -i`

With ninja

```sh
mkdir build
cd build
cmake -G Ninja ..
ninja # compile it all
```

With make

```sh
mkdir build
cd build
cmake ..
make # compile it all
```

You can also list targets with 

```
ninja -t targets

# and build a specific target with
ninja <target>
```
