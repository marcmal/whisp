# 🕵️‍♂️ whisp - Steganography CLI Tool

**whisp** is a modern C++ command-line application for hiding and extracting files inside image files using steganography techniques. It supports both **RGB LSB encoding** and **alpha-channel encoding**, and uses a clean modular design with support for unit testing, logging, and rich CLI parsing.

## ✨ Features

- 🔐 **Hide any file** inside PNG or BMP images
- 🎨 **Two encoding methods**:
  - LSB in RGB channels (1/2/4 bits configurable)
  - Alpha channel embedding
- 🧠 **Automatic decoding** using embedded metadata
- 🧪 Written in **modern C++** with clean code and unit tests
- 📦 Uses libraries: `gtest`, `CLI11`, `CImg`, `spdlog`
- 📦 **Dependency management via [vcpkg](https://github.com/microsoft/vcpkg)**

## 📦 Dependencies

This project uses [vcpkg](https://github.com/microsoft/vcpkg) to manage dependencies. The toolchain file is already integrated via CMake presets.

You can find the preset toolchain settings in:

```
cmake/presets/toolchains/vcpkg.json
```

## 🛠️ Build Instructions

This project uses **CMake Presets** and **Ninja Multi-Config**. You can configure and build the project as follows:

### List available presets

```bash
cmake --list-presets
```

### Configure the project

```bash
cmake --preset <PRESET_NAME>
```

### Build a specific target

```bash
cmake --build --preset <PRESET_NAME> --target <TARGET>
```

## 🚀 Usage

### 🔏 Encode a file using RGB LSB encoding

```bash
whisp encode --image-file input.png --secret-file secret.txt rgb --bits 2
```

- `--bits` can be `1`, `2`, or `4`

### 🫥 Encode a file using alpha channel

```bash
whisp encode --image-file input.png --secret-file secret.txt alpha
```

The output will be saved as:

```
encoded_input.png
```

### 🕵️ Decode a hidden file from an image

```bash
whisp decode --image-file encoded_input.png
```

The tool will detect the encoding method and extract the hidden file.

## 🧪 Testing

To run unit tests:

```bash
cmake --build --preset <PRESET_NAME> --target whisp_tests
ctest --preset <PRESET_NAME>
```

## ⚙️ CMake Configuration Options

You can enable advanced tooling and analysis with the following options when configuring with CMake:

- `-DANALYSIS_ENABLE=ON` – Enables **clang-tidy** analysis on all targets
- `-DSANITIZERS_ENABLE=ON` – Enables **runtime sanitizers** (ASan, UBSan, etc.)
- `-DCOVERAGE_ENABLE=ON` – Enables **code coverage** for unit tests and adds a `coverage` target

> 🧪 **Note**: Coverage collection currently works **only with GCC**.

### 🧾 Presets Overview

You can use the following CMake presets depending on your compiler:

- `x64-linux-clang-*` – For building with **Clang**
- `x64-linux-gcc-*` – For building with **GCC**

Use these presets with the `--preset` flag during configuration and build steps.

## 🧠 How It Works

- For **RGB LSB**, the tool stores bits of the file into the least significant bits of the image's channels.
- For **Alpha**, it stores data directly into the alpha channel of the image.
- A small **header** is embedded into the image to identify the method and bit-depth used, which allows correct decoding.


## 📜 License

MIT License

## ✍️ Author

[github.com/marmal95](https://github.com/marmal95)