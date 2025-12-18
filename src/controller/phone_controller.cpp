#include "./phone_controller.hpp"

void registerPhoneRoutes(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints) {
    // Samsung endpoint
    CROW_ROUTE(app, "/samsung")([&phoneEndpoints]() {
        try {
            crow::response res;
            res.code = 200;
            res.set_header("Content-Type", "application/json");
            res.body = phoneEndpoints.getSamsung().dump();
            return res;
        } catch (const std::exception& e) {
            return crow::response(
                500,
                std::string("Internal Server Error: ") + e.what()
            );
        }
    });

}
