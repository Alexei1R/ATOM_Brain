
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
