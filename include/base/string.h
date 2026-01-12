#pragma once

#include <string>
#include <vector>
#include <format>
#include <concepts>

// 문자열을 소문자로 변환
std::string MakeLower(std::string str);

// 구분자로 문자열 분리 (여러 구분자 - 메인 구현)
std::vector<std::string> Split(std::string_view str, const std::vector<char>& delimiters);

// 구분자로 문자열 분리 (문자열의 각 문자를 구분자로)
std::vector<std::string> Split(std::string_view str, std::string_view delimiters);

// 구분자로 문자열 분리 (단일 구분자)
std::vector<std::string> Split(std::string_view str, char delimiter);

