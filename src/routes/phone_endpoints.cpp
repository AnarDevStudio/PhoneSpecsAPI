#include "phone_endpoints.hpp"

crow::json::wvalue PhoneBrandEndpoints::getApple() {
    crow::json::wvalue result;
    result["brand"] = "Apple";
    result["models"] = {"iPhone 13", "iPhone 12", "iPhone SE"};
    return result;
}