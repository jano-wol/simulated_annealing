# Simulated Annealing Studio



## Project Setup for Windows

### Prerequisites

#### 1. Install Visual Studio
Ensure you have **Visual Studio 2019 (version 16.11 or later)** or **Visual Studio 2022** installed. These versions have full support for C++20.

During installation, please check the following components:
- **Desktop development with C++** workload.
- **C++ CMake tools for Windows** (found under the **Individual components** tab).
- **MSVC toolset**: Ensure the appropriate toolset for your version is installed:
  - **v142** for Visual Studio 2019.
  - **v143** for Visual Studio 2022.

### 2. Install Dependencies with vcpkg
vcpkg is a package manager for C++ libraries that makes it easy to install and integrate external libraries into your project.

#### Install vcpkg
1. Clone the **vcpkg** repository from GitHub:
   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   ```
2. Navigate into the `vcpkg` directory:
   ```bash
   cd vcpkg
   ```
3. Run the bootstrap script to build **vcpkg**:
   - On **Windows** (PowerShell):
     ```powershell
     .bootstrap-vcpkg.bat
     ```

#### Install Required Libraries
Once **vcpkg** is set up, use it to install the required libraries for the project: **glfw** and **opengl**.

1. Install **glfw**:
   ```bash
   .vcpkg install glfw3
   ```
2. Install **opengl**:
   ```bash
   .cpkg install opengl
   ```

#### Integrate vcpkg with Visual Studio
To make **vcpkg** available in Visual Studio, run the following command:
```bash
.vcpkg integrate install
```
This command ensures that **vcpkg** is automatically recognized by Visual Studio and CMake during the build process.

### 3. Clone the Repository
Clone the project repository using the following command:
```bash
git clone https://github.com/jano-wol/simulated_annealing.git
```

## CMake Configuration

The project includes a **CMakePresets.json** file to help with configuration. Before opening the repository in Visual Studio, review the settings in this file.

### Check the CMake Preset

In the **CMakePresets.json**, make sure the paths to **vcpkg** are correct. If needed, adjust the following line to point to your local **vcpkg** installation:

```json
"CMAKE_TOOLCHAIN_FILE": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### Open the Project in Visual Studio

1. Open the repository folder in Visual Studio using **Open Local Folder**.
2. Visual Studio will automatically configure the project using CMake.
3. Once configured, select **Build > Build All** to build all targets.

Visual Studio should handle the rest without requiring additional setup steps. If you encounter any issues, ensure that your Visual Studio installation meets the requirements mentioned above.
