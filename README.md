# ATOM

> RC autonomus car.

Components:

- `app`.
- `libs/*`.


## Getting Started

Install necesar tools
```bash
# Install minimal prerequisites 
sudo apt update && sudo apt install -y cmake g++ wget unzip
sudo apt install clang
sudo apt-get install ninja-build
```
```bash
#GStreamer
sudo apt install gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav
```

Build opencv and opencv contrib 
```bash

# Install minimal prerequisites 
sudo apt update && sudo apt install -y cmake g++ wget unzip
# Download and unpack sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip
unzip opencv.zip
unzip opencv_contrib.zip
# Create build directory and switch into it
mkdir -p build && cd build

#Release 
cmake -DCMAKE_BUILD_TYPE=Release -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules ../opencv-4.x
#Debug 
cmake -DCMAKE_BUILD_TYPE=Debug -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules ../opencv-4.x

```



Before build install
```bash
# Valve GNS libs
sudo apt-get install libssl-dev
sudo apt-get install libprotobuf-dev protobuf-compiler
# Opengl related libs
sudo apt-get install libx11-dev
sudo apt-get install libglfw3 libglfw3-dev
sudo apt-get install libgl1-mesa-dev
sudo apt-get install libxrandr-dev
sudo apt-get install libxinerama-dev
sudo apt-get install libxcursor-dev
sudo apt-get install libxi-dev
```


<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with 

```bash
git clone --recursive https://github.com/Alexei1R/ATOM_Brain
```


If the repository was cloned non-recursively previously, use 
```bash
git submodule update --init
```
to clone the necessary submodules.

<ins>**2. Configuring the dependencies:**</ins>

## Usage
The preferred way to work on the project is in clion or

Create the `build` directory and configure the build system and run make:

```bash
mkdir build & cd build & cmake .. & make -j$(nproc)
```

To run the project cd `build` and run execurable

