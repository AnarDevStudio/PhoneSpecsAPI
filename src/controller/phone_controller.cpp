#include "./phone_controller.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>

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


auto normalize = [](const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    result.erase(std::remove(result.begin(), result.end(), '-'), result.end());
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
};

static crow::response resolveModel(
    const crow::request& req,
    const nlohmann::json& full_json,
    const std::string& model_param)
{
    std::string decoded_model = urlDecode(model_param);
    std::string norm_query    = normalize(decoded_model);

    for (auto& phone : full_json["phones"]) {
        if (normalize(phone["model"].get<std::string>()) == norm_query) {
            nlohmann::json response_json = phone;

            auto query = req.url_params.get("query");
            if (query) {
                std::string q = query;
                if (response_json.contains(q))
                    response_json = response_json[q];
                else
                    return crow::response(404, R"({"error":"Query field not found"})");
            }

            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = response_json.dump();
            return res;
        }
    }

    std::vector<nlohmann::json> matches;
    for (auto& phone : full_json["phones"]) {
        std::string norm_model = normalize(phone["model"].get<std::string>());
        if (norm_model.find(norm_query) != std::string::npos) {
            matches.push_back(phone);
        }
    }

    if (matches.empty())
        return crow::response(404, R"({"error":"Model not found"})");

    if (matches.size() == 1) {
        nlohmann::json response_json = matches[0];

        auto query = req.url_params.get("query");
        if (query) {
            std::string q = query;
            if (response_json.contains(q))
                response_json = response_json[q];
            else
                return crow::response(404, R"({"error":"Query field not found"})");
        }

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = response_json.dump();
        return res;
    }

    nlohmann::json suggestions = nlohmann::json::array();
    for (auto& m : matches)
        suggestions.push_back(m["model"]);

    nlohmann::json result;
    result["matches"] = suggestions;
    result["count"]   = matches.size();
    result["hint"]    = "Multiple models found. Please refine your search.";

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.body = result.dump();
    return res;
}

void registerRandomRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/random")
    ([&phoneEndpoints]() {
        std::vector<nlohmann::json> allPhones;

        auto collect = [&](const nlohmann::json& data) {
            for (auto& phone : data["phones"]) {
                allPhones.push_back(phone);
            }
        };

        collect(phoneEndpoints.getSamsung());
        collect(phoneEndpoints.getApple());
        collect(phoneEndpoints.getXiaomi());
        collect(phoneEndpoints.getGoogle());
        collect(phoneEndpoints.getNokia());
        collect(phoneEndpoints.getHuawei());

        if (allPhones.empty()) {
            return crow::response(404, R"({"error":"No phones available"})");
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, allPhones.size() - 1);

        nlohmann::json randomPhone = allPhones[dist(gen)];

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = randomPhone.dump();
        return res;
    });
}

void registerAllRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/all")
    ([&phoneEndpoints]() {
        nlohmann::json result;
        result["phones"] = nlohmann::json::array();

        auto addPhones = [&](const nlohmann::json& data) {
            for (auto& phone : data["phones"]) {
                result["phones"].push_back(phone);
            }
        };

        addPhones(phoneEndpoints.getSamsung());
        addPhones(phoneEndpoints.getApple());
        addPhones(phoneEndpoints.getXiaomi());
        addPhones(phoneEndpoints.getGoogle());
        addPhones(phoneEndpoints.getNokia());
        addPhones(phoneEndpoints.getHuawei());

        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = result.dump();
        return res;
    });
}

void registerSamsungRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/samsung")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getSamsung().dump();
        return res;
    });

    CROW_ROUTE(app, "/samsung/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getSamsung(), model_param);
    });
}

void registerAppleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/apple")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getApple().dump();
        return res;
    });

    CROW_ROUTE(app, "/apple/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getApple(), model_param);
    });
}

void registerXiaomiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/xiaomi")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getXiaomi().dump();
        return res;
    });

    CROW_ROUTE(app, "/xiaomi/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getXiaomi(), model_param);
    });
}

void registerGoogleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/google")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getGoogle().dump();
        return res;
    });

    CROW_ROUTE(app, "/google/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getGoogle(), model_param);
    });
}

void registerNokiaRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/nokia")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getNokia().dump();
        return res;
    });

    CROW_ROUTE(app, "/nokia/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getNokia(), model_param);
    });
}

void registerHuaweiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/huawei")
    ([&phoneEndpoints]() {
        crow::response res;
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        res.body = phoneEndpoints.getHuawei().dump();
        return res;
    });

    CROW_ROUTE(app, "/huawei/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getHuawei(), model_param);
    });
}