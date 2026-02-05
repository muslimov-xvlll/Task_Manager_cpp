#pragma once
#include <string>
#include <windows.h>

// Цвета
enum class UIColor {
    Default = 7,
    Header = 11, // голубой
    Success = 10, // зелёный
    Warning = 14, // жёлтый
    Error = 12, // красный
    Frame = 9   // синий
};

// Установка цвета
void uiSetColor(UIColor color);

// Рамки и линии
void uiLine(int length = 50);
void uiHeader(const std::string& text);
void uiSection(const std::string& text);

// Сообщения
void uiSuccess(const std::string& text);
void uiError(const std::string& text);

// Табличные строки
void uiTableRow(const std::string& col1,
    const std::string& col2,
    const std::string& col3,
    const std::string& col4,
    const std::string& col5);
