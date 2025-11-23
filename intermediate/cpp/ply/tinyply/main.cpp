#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <memory>
#include "tinyply.h"

using namespace tinyply;

struct float3 {
    float x, y, z;
};

struct uint3 {
    uint32_t x, y, z;
};

// Helper function to read entire file into memory
std::vector<uint8_t> read_file_binary(const std::string& pathToFile) {
    std::ifstream file(pathToFile, std::ios::binary);
    std::vector<uint8_t> fileBufferBytes;

    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size_t sizeBytes = file.tellg();
        file.seekg(0, std::ios::beg);
        fileBufferBytes.resize(sizeBytes);
        if (file.read((char*)fileBufferBytes.data(), sizeBytes)) return fileBufferBytes;
    }
    else throw std::runtime_error("could not open binary ifstream to path " + pathToFile);
    return fileBufferBytes;
}

struct memory_buffer : public std::streambuf {
    char* p_start{nullptr};
    char* p_end{nullptr};
    size_t size;

    memory_buffer(char const* first_elem, size_t size)
        : p_start(const_cast<char*>(first_elem)), p_end(p_start + size), size(size) {
        setg(p_start, p_start, p_end);
    }

    pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override {
        if (dir == std::ios_base::cur) gbump(static_cast<int>(off));
        else setg(p_start, (dir == std::ios_base::beg ? p_start : p_end) + off, p_end);
        return gptr() - p_start;
    }

    pos_type seekpos(pos_type pos, std::ios_base::openmode which) override {
        return seekoff(pos, std::ios_base::beg, which);
    }
};

struct memory_stream : virtual memory_buffer, public std::istream {
    memory_stream(char const* first_elem, size_t size)
        : memory_buffer(first_elem, size), std::istream(static_cast<std::streambuf*>(this)) {}
};

void convertPlyToObj(const std::string& inputFile, const std::string& outputFile) {
    try {
        // Read entire file into memory (following tinyply example approach)
        std::vector<uint8_t> byte_buffer = read_file_binary(inputFile);
        std::unique_ptr<std::istream> file_stream;
        file_stream.reset(new memory_stream((char*)byte_buffer.data(), byte_buffer.size()));
        
        if (!file_stream || file_stream->fail()) {
            throw std::runtime_error("file_stream failed to open " + inputFile);
        }

        file_stream->seekg(0, std::ios::end);
        file_stream->seekg(0, std::ios::beg);

        PlyFile file;
        file.parse_header(*file_stream);

        std::cout << "PLY Header Information:\n";
        std::cout << "Comments: ";
        for (auto c : file.get_comments()) std::cout << c << " ";
        std::cout << std::endl;
        std::cout << "Elements: ";
        for (auto e : file.get_elements()) {
            std::cout << e.name << " (" << e.size << ") ";
        }
        std::cout << std::endl;

        // Request vertex data
        std::shared_ptr<PlyData> vertices;
        std::shared_ptr<PlyData> normals;
        std::shared_ptr<PlyData> faces;

        try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
        catch (const std::exception& e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
        catch (const std::exception& e) { /* Normals are optional */ }

        try { faces = file.request_properties_from_element("face", { "vertex_indices" }, 3); }
        catch (const std::exception& e) {
            try { faces = file.request_properties_from_element("face", { "vertex_index" }, 3); }
            catch (const std::exception& e2) { std::cerr << "tinyply exception: " << e2.what() << std::endl; }
        }

        file.read(*file_stream);

        // Write OBJ file
        std::ofstream objFile(outputFile);
        if (!objFile.is_open()) {
            throw std::runtime_error("Failed to open output file: " + outputFile);
        }

        objFile << "# Converted from " << inputFile << std::endl;
        objFile << "# Using tinyply library\n" << std::endl;

        // Write vertices
        if (vertices && vertices->count > 0) {
            const size_t numVerticesBytes = vertices->buffer.size_bytes();
            std::vector<float3> verts(vertices->count);
            std::memcpy(verts.data(), vertices->buffer.get(), numVerticesBytes);

            for (size_t i = 0; i < verts.size(); ++i) {
                objFile << "v " << verts[i].x << " " << verts[i].y << " " << verts[i].z << std::endl;
            }
            std::cout << "Wrote " << verts.size() << " vertices" << std::endl;
        }

        // Write normals
        if (normals && normals->count > 0) {
            const size_t numNormalsBytes = normals->buffer.size_bytes();
            std::vector<float3> norms(normals->count);
            std::memcpy(norms.data(), normals->buffer.get(), numNormalsBytes);

            for (size_t i = 0; i < norms.size(); ++i) {
                objFile << "vn " << norms[i].x << " " << norms[i].y << " " << norms[i].z << std::endl;
            }
            std::cout << "Wrote " << norms.size() << " normals" << std::endl;
        }

        // Write faces
        if (faces && faces->count > 0) {
            const size_t numFacesBytes = faces->buffer.size_bytes();
            std::vector<uint3> faceIndices(faces->count);
            std::memcpy(faceIndices.data(), faces->buffer.get(), numFacesBytes);

            for (size_t i = 0; i < faceIndices.size(); ++i) {
                // OBJ indices are 1-based, PLY indices are 0-based
                objFile << "f " << (faceIndices[i].x + 1) << " " 
                        << (faceIndices[i].y + 1) << " " 
                        << (faceIndices[i].z + 1) << std::endl;
            }
            std::cout << "Wrote " << faceIndices.size() << " faces" << std::endl;
        }

        objFile.close();
        std::cout << "\nSuccessfully converted " << inputFile << " to " << outputFile << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.ply> <output.obj>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    try {
        convertPlyToObj(inputFile, outputFile);
    } catch (const std::exception& e) {
        std::cerr << "Conversion failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
