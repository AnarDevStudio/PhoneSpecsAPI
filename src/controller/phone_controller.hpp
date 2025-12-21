#pragma once
#include "../include/crow_all.h"
#include "../routes/phone_endpoints.hpp"


void registerSamsungRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);
void registerAppleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);