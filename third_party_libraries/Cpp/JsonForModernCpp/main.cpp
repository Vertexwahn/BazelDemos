#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;

int main() {
	json performance_json_data;
	performance_json_data["application"] = "okapi.cli";
	std::cout << performance_json_data.dump(4) << std::endl;
}
