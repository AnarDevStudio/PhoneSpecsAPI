#include "include/crow_all.h"
#include "./controller/phone_controller.hpp"

int main() {
    crow::Crow<> app;
    PhoneBrandEndpoints phoneEndpoints;

    registerSamsungRoute(app, phoneEndpoints);
    registerAppleRoute(app, phoneEndpoints);

    app.port(8080).multithreaded().run();

    return 0;
}
