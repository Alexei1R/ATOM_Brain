# ATOM

> RC autonomus car.

Components:

- `app`.
- `libs/*`.


## Getting Started
<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Configuring the dependencies:**</ins>

## Usage

Create the `build` directory and configure the build system:

```bash
cmake -B build # Or `cmake -D BUILD_TESTING=OFF -B build` to skip tests. 
```

Build the project:

```bash
cmake --build build
```

```bash
cmake -E chdir build ctest
```