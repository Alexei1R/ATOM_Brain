# ATOM

> RC autonomus car.

Components:

- `app`.
- `libs/*`.


## Getting Started

#Before clone follow prebuild instructions in folder Info
[Prebuild Instructions](Info/build_opencv.md)



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


## Usage
The preferred way to work on the project is in clion or

Create the `build` directory, configure the build system and run make:

```bash
mkdir build & cd build & cmake .. & make -j$(nproc)
```

To run the project cd `build` and run execurable

