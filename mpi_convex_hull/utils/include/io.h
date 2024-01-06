#pragma once

#include <ostream>

constexpr const auto* kAnsiColorRed = "\x1b[31m";
constexpr const auto* kAnsiColorGreen = "\x1b[32m";
constexpr const auto* kAnsiColorYellow = "\x1b[33m";
// constexpr const auto* kAnsiColorBlue = "\x1b[34m";
// constexpr const auto* kAnsiColorMagenta = "\x1b[35m";
// constexpr const auto* kAnsiColorCyan = "\x1b[36m";
// constexpr const auto* kAnsiColorWhite = "\x1b[37m";
constexpr const auto* kAnsiColorReset = "\x1b[0m";

template <typename... Args>
void Print(std::ostream& os, const char* color, Args&&... args) {
  os << color;
  ((os << std::forward<Args>(args)), ...);
  os << kAnsiColorReset;
}

template <typename... Args>
void PrintRed(std::ostream& os, Args&&... args) {
  Print(os, kAnsiColorRed, std::forward<Args>(args)...);
}

template <typename... Args>
void PrintGreen(std::ostream& os, Args&&... args) {
  Print(os, kAnsiColorGreen, std::forward<Args>(args)...);
}

template <typename... Args>
void PrintYellow(std::ostream& os, Args&&... args) {
  Print(os, kAnsiColorYellow, std::forward<Args>(args)...);
}
