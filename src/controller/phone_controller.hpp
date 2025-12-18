#pragma once
#include "../include/crow_all.h"
#include "../routes/phone_endpoints.hpp"

// Crow app'i ve endpoint objesini alıp route'ları ekleyen fonksiyon
void registerPhoneRoutes(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);
