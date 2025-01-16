# Simulated Annealing

This is a personal project aimed at exploring and showcasing [Simulated Annealing](https://en.wikipedia.org/wiki/Simulated_annealing). It is designed for two main use cases:

1. For those interested in understanding Simulated Annealing, the repository provides programmed use cases that demonstrate its principles. The examples allow for parameter adjustments and include visualizations to illustrate the optimization process and its outcomes.

2. For C++ developers seeking to apply Simulated Annealing to specific problems, the repository offers a performant framework. By implementing a minimal set of problem-specific functions (such as ``getEnergy``, ``generateMove``, ``fromString`` etc.), users gain access to features like a graphical UI for visualization, process diagnostics, solution tracking, and parallel thread handling.

Cross-platform development is supported, with installation instructions for both Linux and Windows.

## Setup

### Linux
```bash
# 1. Update package lists
sudo apt update

# 2. Install required packages
sudo apt install build-essential cmake ninja-build git libglfw3-dev libopengl-dev

# 3. Clone the repository to a desired folder and navigate to it
git clone https://github.com/jano-wol/simulated_annealing.git
cd simulated_annealing

# 4. Configure and build the project (change 'release' to 'debug' to get debug build)
./source/scripth/configure.sh release
./source/scripth/build.sh release

# 5. Run the UI
./build/release/bin/sa_studio
```

### Windows

#### 1. Prerequisites
``Visual Studio 2019`` (version 16.11 or later) or ``Visual Studio 2022`` is required, along with the ``C++ CMake tools for Windows`` component. To check if it is installed, open the Visual Studio Installer, select 'Modify', go to 'Individual Components', and search for 'C++ CMake tools for Windows'.

#### 2. Install dependencies with vcpkg
```bash
# 1. This step is only needed if vcpkg is not already available on your system
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat

# 2. Install the required packages
vcpkg install glfw3 opengl
vcpkg integrate install
```

#### 3. Build
1. Clone the repository to a desired location:
```bash
git clone https://github.com/jano-wol/simulated_annealing.git
```

2. Review the [CMakePresets.json](./CMakePresets.json) file. If the root folder of vcpkg was not cloned to C:\, adapt the ``CMAKE_TOOLCHAIN_FILE`` path values twice (once for each build type).

3. Open ``Visual Studio`` and choose 'Open a local folder', then select the root folder of ``simulated_annealing``. Configuration should run automatically and complete successfully. Once the configuration is ready, build the project by selecting ``Build > Build All``.

4. Run the UI:
```bash
cd simulated_annealing
build\release\bin\sa_studio.exe
``` 

