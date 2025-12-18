#include "include/crow_all.h"
#include "./controller/phone_controller.hpp"

int main() {
    crow::Crow<> app;
    PhoneBrandEndpoints phoneEndpoints;

    registerPhoneRoutes(app, phoneEndpoints);

    app.port(8080).multithreaded().run();

    return 0;
}
