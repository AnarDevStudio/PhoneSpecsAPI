#ifndef PHONE_ENDPOINTS_HPP
#define PHONE_ENDPOINTS_HPP

#include "../include/nlohmann/json.hpp"
#include <string>

class PhoneBrandEndpoints {
public:
    nlohmann::json getSamsung();
    nlohmann::json getApple();
    nlohmann::json getXiaomi();
    nlohmann::json getGoogle();
    nlohmann::json getHuawei();
    nlohmann::json getNokia();

private:
    nlohmann::json readJsonFile(const std::string& path);
    const std::string file_path = "data/"; // programı çalıştırdığın dizine göre
};

#endif
