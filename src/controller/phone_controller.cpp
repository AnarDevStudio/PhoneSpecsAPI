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

static const nlohmann::json* getNestedValue(const nlohmann::json& node, const std::string& path) {
    const nlohmann::json* current = &node;
    std::string segment;
    std::istringstream stream(path);
    while (std::getline(stream, segment, '.')) {
        if (!current->is_object() || !current->contains(segment))
            return nullptr;
        current = &(*current)[segment];
    }
    return current;
}

static bool matchesFilters(const nlohmann::json& phone, const crow::request& req) {
    for (auto& param : req.url_params.keys()) {
        std::string key(param);
        if (key == "query") continue;
        auto raw_val = req.url_params.get(key);
        if (!raw_val) continue;
        std::string filter_val = urlDecode(std::string(raw_val));

        const nlohmann::json* target = getNestedValue(phone, key);
        if (!target) return false;

        if (target->is_array()) {
            bool found = false;
            for (auto& elem : *target) {
                std::string elem_str = elem.is_string() ? elem.get<std::string>() : elem.dump();
                if (normalize(elem_str) == normalize(filter_val)) { found = true; break; }
            }
            if (!found) return false;
        } else if (target->is_object()) {
            return false;
        } else {
            std::string val_str = target->is_string() ? target->get<std::string>() : target->dump();
            if (normalize(val_str) != normalize(filter_val)) return false;
        }
    }
    return true;
}

static crow::response applyFiltersAndRespond(const crow::request& req, const nlohmann::json& full_json) {
    nlohmann::json result = nlohmann::json::array();
    for (auto& phone : full_json["phones"]) {
        if (matchesFilters(phone, req))
            result.push_back(phone);
    }
    if (result.empty())
        return crow::response(404, R"({"error":"No phones match the given filters"})");
    nlohmann::json response_json;
    response_json["phones"] = result;
    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "application/json");
    res.body = response_json.dump();
    return res;
}

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
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getSamsung();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/samsung/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getSamsung(), model_param);
    });
}

void registerAppleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/apple")
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getApple();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/apple/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getApple(), model_param);
    });
}

void registerXiaomiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/xiaomi")
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getXiaomi();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/xiaomi/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getXiaomi(), model_param);
    });
}

void registerGoogleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/google")
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getGoogle();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/google/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getGoogle(), model_param);
    });
}

void registerNokiaRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/nokia")
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getNokia();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/nokia/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getNokia(), model_param);
    });
}

void registerHuaweiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    CROW_ROUTE(app, "/huawei")
    ([&phoneEndpoints](const crow::request& req) {
        auto data = phoneEndpoints.getHuawei();
        if (req.url_params.keys().empty())
            return crow::response(200, data.dump());
        return applyFiltersAndRespond(req, data);
    });

    CROW_ROUTE(app, "/huawei/<string>")
    ([&phoneEndpoints](const crow::request& req, const std::string& model_param) {
        return resolveModel(req, phoneEndpoints.getHuawei(), model_param);
    });
}