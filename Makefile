all:
	g++ ./src/main.cpp ./src/routes/phone_endpoints.cpp ./src/controller/phone_controller.cpp -Iinclude -std=c++17 -o app