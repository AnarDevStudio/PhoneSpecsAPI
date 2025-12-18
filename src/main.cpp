#include "include/crow_all.h"
#include "./controller/phone_controller.hpp"

int main() {
    crow::Crow<> app;
    PhoneBrandEndpoints phoneEndpoints;

    // Route'ları register et
    registerPhoneRoutes(app, phoneEndpoints);

    // Server başlat
    app.port(8080).multithreaded().run();

    return 0;
}
