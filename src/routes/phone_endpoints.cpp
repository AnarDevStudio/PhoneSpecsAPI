#include "phone_endpoints.hpp"
#include <fstream>
#include <stdexcept>

nlohmann::json PhoneBrandEndpoints::readJsonFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("JSON dosyasi acilamadi: " + path);
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        throw std::runtime_error("JSON parse hatasi: " + std::string(e.what()));
    }


    return data;
}

nlohmann::json PhoneBrandEndpoints::getSamsung() {
    return readJsonFile(file_path + "samsung_data.json");
}
