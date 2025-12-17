#include "./routes/phone_endpoints.hpp"

int main() {
    crow::App<> app;

    routes::register_phone_endpoints(app);

    app.port(8080).multithreaded().run();
    return 0;
}