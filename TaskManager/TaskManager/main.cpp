#include <iostream>
#include <windows.h>
#include "Application.hpp"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    setlocale(LC_ALL, "rus");

    Application app;
    app.run();

    return 0;
}
