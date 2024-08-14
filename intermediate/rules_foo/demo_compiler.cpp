#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <value>\n";
        return 1;
    }

    std::string filename = argv[1];
    double value = std::stod(argv[2]);

    std::ofstream headerFile(filename);
    if (!headerFile.is_open()) {
        std::cerr << "Error: Unable to create file: " << filename << std::endl;
        return 1;
    }

    headerFile << "#ifndef CONSTANTS_H\n";
    headerFile << "#define CONSTANTS_H\n\n";
    headerFile << "#define PI " << value << "\n\n";
    headerFile << "#endif // CONSTANTS_H\n";

    headerFile.close();

    std::cout << "Header file '" << filename << "' generated successfully.\n";

    return 0;
}
