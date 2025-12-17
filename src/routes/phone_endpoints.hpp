#ifndef ROUTES_PHONE_ENDPOINTS_HPP
#define ROUTES_PHONE_ENDPOINTS_HPP
#include <string>
#include "../include/crow_all.h"
#include "../include/nlohmann/json.hpp"

namespace routes {
    void register_phone_endpoints(crow::App<>& app);
}

class PhoneBrandEndpoints {
public:
    crow::json::wvalue getApple();
    crow::json::wvalue getSamsung();
    crow::json::wvalue getXiaomi();
    crow::json::wvalue getGoogle();
    crow::json::wvalue getHuawei();
    crow::json::wvalue getNokia();
};




#endif // ROUTES_PHONE_ENDPOINTS_HPP
