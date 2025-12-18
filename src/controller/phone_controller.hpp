#pragma once
#include "../include/crow_all.h"
#include "../routes/phone_endpoints.hpp"


void registerPhoneRoutes(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);
