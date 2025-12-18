all:
	g++ ./src/main.cpp ./src/routes/phone_endpoints.cpp -Iinclude -std=c++17 -o app