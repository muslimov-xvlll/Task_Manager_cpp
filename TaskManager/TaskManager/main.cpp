#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Application.hpp"

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    #endif

    setlocale(LC_ALL, "rus");

    Application app;
    app.run();

    return 0;
}