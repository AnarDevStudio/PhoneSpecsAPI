#include "./phone_controller.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>


std::string urlDecode(const std::string& str) {
    std::string result;
    char ch;
    int i, ii;
    for (i = 0; i < str.length(); i++) {
        if (str[i] == '%') {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            result += ch;
            i = i + 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

void registerPhoneRoutes(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {

    CROW_ROUTE(app, "/samsung")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getSamsung().dump();
        return res;
    });


    CROW_ROUTE(app, "/samsung/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param){
        nlohmann::json full_json = phoneEndpoints.getSamsung();
        nlohmann::json response_json;

        auto normalize = [](const std::string& s){
            std::string result = s;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
            result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
            return result;
        };

        std::string decoded_model = urlDecode(model_param);
        std::string norm_model = normalize(decoded_model);
        bool found = false;

        for (auto& phone : full_json["phones"]) {
            if (normalize(phone["model"].get<std::string>()) == norm_model) {
                response_json = phone;
                found = true;
                break;
            }
        }

        if (!found)
            return crow::response(404, R"({"error":"Model not found"})");

        auto query = req.url_params.get("query"); 
        if (query) {
            std::string q = query;

            if (response_json.contains(q)) {
                response_json = response_json[q];
            } else {
                return crow::response(404, R"({"error":"Query field not found"})");
            }
        }

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = response_json.dump();
        return res;
    });

}