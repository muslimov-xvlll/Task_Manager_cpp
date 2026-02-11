#include "UI.hpp"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

void uiSetColor(UIColor color) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, static_cast<int>(color));
#else
    switch (color) {
        case UIColor::Default: std::cout << "\033[0m"; break;
        case UIColor::Header:  std::cout << "\033[36m"; break; // голубой
        case UIColor::Success: std::cout << "\033[32m"; break; // зелёный
        case UIColor::Warning: std::cout << "\033[33m"; break; // жёлтый
        case UIColor::Error:   std::cout << "\033[31m"; break; // красный
        case UIColor::Frame:   std::cout << "\033[34m"; break; // синий
    }
#endif
}

void uiResetColor() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 7);
#else
    std::cout << "\033[0m";
#endif
}

void uiLine(int length) {
    uiSetColor(UIColor::Frame);
    std::cout << "+";
    for (int i = 0; i < length - 2; i++) std::cout << "-";
    std::cout << "+\n";
    uiSetColor(UIColor::Default);
}

void uiHeader(const std::string& text) {
    uiLine(60);
    uiSetColor(UIColor::Header);
    std::cout << "| " << std::left << std::setw(57) << text << "|\n";
    uiSetColor(UIColor::Default);
    uiLine(60);
}

void uiSuccess(const std::string& text) {
    uiSetColor(UIColor::Success);
    std::cout << "[OK] " << text << "\n";
    uiSetColor(UIColor::Default);
}

void uiSection(const std::string& text) {
    uiSetColor(UIColor::Frame);
    std::cout << "+------------------------------------------------------------+\n";
    uiSetColor(UIColor::Header);
    std::cout << "| " << text << "\n";
    uiSetColor(UIColor::Frame);
    std::cout << "+------------------------------------------------------------+\n";
    uiSetColor(UIColor::Default);
}


void uiError(const std::string& text) {
    uiSetColor(UIColor::Error);
    std::cout << "[ERR] " << text << "\n";
    uiSetColor(UIColor::Default);
}

void uiTableRow(const std::string& col1,
    const std::string& col2,
    const std::string& col3,
    const std::string& col4,
    const std::string& col5)
{
    uiSetColor(UIColor::Frame);
    std::cout << "| ";
    uiSetColor(UIColor::Default);

    std::cout << std::left
        << std::setw(4) << col1 << " | "
        << std::setw(12) << col2 << " | "
        << std::setw(20) << col3 << " | "
        << std::setw(12) << col4 << " | "
        << std::setw(12) << col5 << " |\n";
}
