#pragma once
#include <string>
#include <memory>
#include <fmt/format.h>

namespace spdlog
{
class logger;
}

namespace base
{

// 로그 레벨
enum class LogLevel
{
    Debug,
    Info,
    Error,
    Always
};

// 로거 설정
struct LoggerConfig
{
    bool enableConsole = true;
    bool enableFile = true;
    std::string logDirectory = "logs";
    std::string logFilePrefix = "server";
    size_t asyncQueueSize = 8192;  // 배치 로깅 큐 크기
    size_t threadPoolSize = 1;     // async 로깅 스레드 수
};

// 로거 초기화
void InitLogger(const LoggerConfig& config);

// 로그 레벨 필터링 설정
void SetLogLevel(LogLevel level);

// 로거 종료 (모든 로그 플러시 후 대기)
void Shutdown();

// 로깅 함수
template<typename... Args>
void Log(const char* category, LogLevel level, fmt::format_string<Args...> fmt, Args&&... args)
{
    extern void LogImpl(const char* category, LogLevel level, const std::string& message);
    LogImpl(category, level, fmt::format(fmt, std::forward<Args>(args)...));
}

// 로깅 함수 (단순 문자열)
void Log(const char* category, LogLevel level, const char* message);

} // namespace base

// 로깅 매크로
#define LOG_DEBUG(category, ...) ::base::Log(category, ::base::LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(category, ...) ::base::Log(category, ::base::LogLevel::Info, __VA_ARGS__)
#define LOG_ERROR(category, ...) ::base::Log(category, ::base::LogLevel::Error, __VA_ARGS__)
#define LOG_ALWAYS(category, ...) ::base::Log(category, ::base::LogLevel::Always, __VA_ARGS__)
