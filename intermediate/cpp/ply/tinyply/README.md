# Tinyply Demo - PLY to OBJ Converter

This demo shows how to use the [tinyply](https://github.com/ddiakopoulos/tinyply) library with Bazel to convert PLY (Polygon File Format) files to OBJ (Wavefront) format.

## Features

- Reads PLY files (binary format recommended)
- Converts vertex positions and normals
- Converts face indices  
- Outputs standard OBJ format files

**Note:** This demo uses tinyply 2.3.4, which has a known issue reading ASCII PLY files. Binary PLY files work correctly.

## Prerequisites

- Bazel (uses Bzlmod for dependency management)

## Building

```bash
bazel build //:ply2obj
```

## Running

Convert a PLY file to OBJ format:

```bash
bazel run //:ply2obj -- <input.ply> <output.obj>
```

Example with the included sample file:

```bash
bazel run //:ply2obj -- $(pwd)/triangle.ply $(pwd)/triangle.obj
```

Or run the built binary directly:

## Sample File

A sample `triangle.ply` file (binary format) is included for testing. It contains a simple triangle mesh with vertex positions and normals.

## How It Works

1. **MODULE.bazel** - Uses Bzlmod to fetch tinyply 2.3.4 from GitHub
2. **bazel/tinyply.BUILD** - Defines how to build the tinyply library
3. **main.cpp** - Implements the conversion logic:
   - Loads PLY file into memory
   - Parses header and requests vertex/normal/face data
   - Reads the PLY data using tinyply
   - Writes OBJ format output

## Input Format

The converter expects binary PLY files with:
- `vertex` element with `x`, `y`, `z` properties
- Optional: `nx`, `ny`, `nz` normal properties  
- `face` element with `vertex_indices` list property

## Output Format

The generated OBJ file contains:
- `v` lines for vertex positions
- `vn` lines for vertex normals (if present in input)
- `f` lines for face indices (1-based indexing)

## Known Limitations

- **ASCII PLY files:** Tinyply 2.3.4 has a parsing bug with ASCII format PLY files. Use binary format instead.
- **Face types:** Only triangular faces are supported

## Troubleshooting

### "unexpected EOF. malformed file?" error

This error typically occurs when trying to read ASCII PLY files. Convert your PLY file to binary format, or use a binary PLY file as input.

### Build issues

Ensure you have a recent version of Bazel installed:

```bash
bazel version
```

The demo uses Bzlmod (MODULE.bazel) for dependency management, which requires Bazel 6.0 or later.
