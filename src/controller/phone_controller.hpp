#pragma once
#include "../include/crow_all.h"
#include "../routes/phone_endpoints.hpp"


void registerSamsungRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);
void registerAppleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);
void registerXiaomiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);  //Selamma hahahahahah
void registerGoogleRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);  //Selamma hahahahahah
void registerNokiaRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);  //Selamma hahahahahah
void registerHuaweiRoute(crow::Crow<>& app, PhoneBrandEndpoints& phoneEndpoints);  //Selamma hahahahahah


