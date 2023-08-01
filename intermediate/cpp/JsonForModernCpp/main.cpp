#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

int main() {
	json json_data;
	json_data["application"] = "myApp";
	json_data["version"] = "1.0.0";
	std::cout << json_data.dump(4) << std::endl;
}
