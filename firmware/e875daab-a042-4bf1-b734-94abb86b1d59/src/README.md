# Implement a GitHub Actions CI/CD workflow to automatically build the firmware project upon pushes to the repository.

## Build Instructions

1.  **Local Development (Linux/macOS):**
    *   Ensure `cmake`, `ninja-build` (or `make`), and `gcc-arm-none-eabi` are installed.
    *   Navigate to the project root.
    *   Create a build directory: `mkdir build`
    *   Change into the build directory: `cd build`
    *   Configure CMake: `cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..` (or `cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..` if using make)
    *   Build the project: `ninja` (or `make`)
    *   The compiled `.elf`, `.hex`, and `.bin` files will be in `build/bin/`.

2.  **GitHub Actions:**
    *   The provided `.github/workflows/main.yml` will automatically handle installation and compilation upon push/pull request to the `main` branch.
    *   No manual build steps are needed for CI; simply push your code to GitHub.

**Important Note on `toolchain.cmake`:**
This example assumes a `toolchain.cmake` file exists at the root of your repository. This file is critical for CMake to correctly identify and use the ARM cross-compiler. The provided `toolchain.cmake` is a minimal example. In a real nRF52 project, this file might also include specific paths to the nRF Connect SDK or nRF5 SDK, define target-specific flags, and potentially include other CMake modules provided by Nordic.