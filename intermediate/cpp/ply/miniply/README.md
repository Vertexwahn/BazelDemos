# PLY to OBJ Converter using Miniply

A simple command-line tool to convert PLY (Polygon File Format) files to OBJ (Wavefront) format, built with [miniply](https://github.com/vilya/miniply) and Bazel.

## Features

- Converts PLY files (both ASCII and binary formats) to OBJ format
- Preserves vertex positions, normals, and face data
- Built with Bazel for reliable, reproducible builds
- Uses the lightweight miniply library for fast PLY parsing

## Prerequisites

- Bazel 8.4.2 or compatible version
- C++ compiler with C++11 support

## Building

Build the converter using Bazel:

```bash
bazel build //:ply2obj
```

## Usage

After building, run the converter with:

```bash
bazel run //:ply2obj -- <input.ply> <output.obj>
```

Or run the binary directly:

```bash
./bazel-bin/ply2obj <input.ply> <output.obj>
```

### Example

```bash
bazel run //:ply2obj -- model.ply model.obj
```

## Output

The converter will:
- Read the input PLY file
- Display information about the file (binary/ASCII, vertex count, face count)
- Write vertex positions (v)
- Write vertex normals (vn) if present
- Write faces (f) with proper indexing

## Project Structure

```
miniply/
├── BUILD.bazel          # Build configuration for the binary
├── MODULE.bazel         # Bazel module with miniply dependency
├── main.cpp             # PLY to OBJ converter implementation
├── README.md            # This file
└── bazel/
    └── miniply.BUILD    # Build configuration for miniply library
```

## Implementation Details

This converter uses the miniply library, which provides:
- Efficient PLY file parsing
- Support for both ASCII and binary PLY formats
- Simple API for extracting vertex and face data

The converter handles:
- Vertex positions (x, y, z)
- Vertex normals (nx, ny, nz) when available
- Triangle faces (vertex indices)
- Proper index conversion (PLY uses 0-based, OBJ uses 1-based indexing)

## Comparison with tinyply

While this project uses miniply, a similar implementation exists using [tinyply](https://github.com/ddiakopoulos/tinyply) in the `../tinyply` directory. Key differences:

- **miniply**: Lightweight, simple API, focused on reading PLY files
- **tinyply**: More feature-rich, supports both reading and writing PLY files

Both libraries handle the PLY format well; choose based on your specific needs.

## License

This converter code is provided as-is for educational and practical use. The miniply library has its own license (see the miniply repository).
