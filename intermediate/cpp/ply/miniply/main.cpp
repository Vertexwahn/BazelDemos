#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "miniply.h"

struct Vertex {
    float x, y, z;
};

struct Normal {
    float nx, ny, nz;
};

struct Face {
    uint32_t v0, v1, v2;
};

void convertPlyToObj(const std::string& inputFile, const std::string& outputFile) {
    try {
        // Open PLY file using miniply
        miniply::PLYReader reader(inputFile.c_str());
        if (!reader.valid()) {
            throw std::runtime_error("Failed to open PLY file: " + inputFile);
        }

        std::cout << "PLY file opened successfully\n";
        
        const char* fileTypeStr = "unknown";
        switch (reader.file_type()) {
            case miniply::PLYFileType::ASCII: fileTypeStr = "ASCII"; break;
            case miniply::PLYFileType::Binary: fileTypeStr = "binary"; break;
            case miniply::PLYFileType::BinaryBigEndian: fileTypeStr = "binary (big endian)"; break;
        }
        std::cout << "File type: " << fileTypeStr << "\n";

        std::vector<Vertex> vertices;
        std::vector<Normal> normals;
        std::vector<Face> faces;
        
        bool hasNormals = false;

        // Read through all elements
        while (reader.has_element() && reader.element_is(miniply::kPLYVertexElement)) {
            if (!reader.load_element()) {
                throw std::runtime_error("Failed to load vertex element");
            }

            uint32_t vertexCount = reader.num_rows();
            std::cout << "Reading " << vertexCount << " vertices...\n";

            // Check for position properties
            uint32_t positionIdx[3];
            bool hasPosition = reader.find_pos(positionIdx);
            
            // Check for normal properties
            uint32_t normalIdx[3];
            hasNormals = reader.find_normal(normalIdx);

            if (!hasPosition) {
                throw std::runtime_error("Vertex element does not contain position data");
            }

            vertices.resize(vertexCount);
            if (hasNormals) {
                normals.resize(vertexCount);
            }

            // Extract vertex positions and normals
            reader.extract_properties(positionIdx, 3, miniply::PLYPropertyType::Float, &vertices[0]);
            
            if (hasNormals) {
                reader.extract_properties(normalIdx, 3, miniply::PLYPropertyType::Float, &normals[0]);
            }

            reader.next_element();
        }

        // Read face data
        if (reader.has_element() && reader.element_is(miniply::kPLYFaceElement)) {
            if (!reader.load_element()) {
                throw std::runtime_error("Failed to load face element");
            }

            uint32_t faceCount = reader.num_rows();
            std::cout << "Reading " << faceCount << " faces...\n";

            uint32_t faceIdx[1];
            bool hasFaces = reader.find_indices(faceIdx);
            
            if (!hasFaces) {
                std::cerr << "Warning: Face element found but no index data\n";
            } else {
                // Get total number of indices we need
                uint32_t totalIndices = reader.sum_of_list_counts(faceIdx[0]);
                
                // Allocate space for all indices
                std::vector<uint32_t> faceIndices(totalIndices);
                
                // Load face indices
                bool extractedOk = reader.extract_list_property(faceIdx[0], miniply::PLYPropertyType::Int, faceIndices.data());
                
                if (extractedOk && faceIndices.size() % 3 == 0) {
                    faces.resize(faceIndices.size() / 3);
                    for (size_t i = 0; i < faces.size(); ++i) {
                        faces[i].v0 = faceIndices[i * 3 + 0];
                        faces[i].v1 = faceIndices[i * 3 + 1];
                        faces[i].v2 = faceIndices[i * 3 + 2];
                    }
                } else {
                    std::cerr << "Warning: Could not extract face indices or non-triangular faces detected\n";
                }
            }
        }

        // Write OBJ file
        std::ofstream objFile(outputFile);
        if (!objFile.is_open()) {
            throw std::runtime_error("Failed to open output file: " + outputFile);
        }

        objFile << "# Converted from " << inputFile << std::endl;
        objFile << "# Using miniply library\n" << std::endl;

        // Write vertices
        for (const auto& v : vertices) {
            objFile << "v " << v.x << " " << v.y << " " << v.z << std::endl;
        }
        std::cout << "Wrote " << vertices.size() << " vertices\n";

        // Write normals
        if (hasNormals && !normals.empty()) {
            for (const auto& n : normals) {
                objFile << "vn " << n.nx << " " << n.ny << " " << n.nz << std::endl;
            }
            std::cout << "Wrote " << normals.size() << " normals\n";
        }

        // Write faces (OBJ uses 1-based indexing, PLY uses 0-based)
        for (const auto& f : faces) {
            if (hasNormals) {
                // Include normal indices (assuming normals per vertex)
                objFile << "f " << (f.v0 + 1) << "//" << (f.v0 + 1) << " "
                        << (f.v1 + 1) << "//" << (f.v1 + 1) << " "
                        << (f.v2 + 1) << "//" << (f.v2 + 1) << std::endl;
            } else {
                objFile << "f " << (f.v0 + 1) << " " << (f.v1 + 1) << " " << (f.v2 + 1) << std::endl;
            }
        }
        std::cout << "Wrote " << faces.size() << " faces\n";

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
