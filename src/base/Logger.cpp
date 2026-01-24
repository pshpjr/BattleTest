#include "base/Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <filesystem>
#include <memory>

namespace base
{

namespace
{
    std::shared_ptr<spdlog::logger> g_logger;
    LogLevel g_minLogLevel = LogLevel::Debug;

    // LogLevel을 spdlog level로 변환
    spdlog::level::level_enum ToSpdlogLevel(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Debug: return spdlog::level::debug;
            case LogLevel::Info: return spdlog::level::info;
            case LogLevel::Error: return spdlog::level::err;
            case LogLevel::Always: return spdlog::level::critical;
            default: return spdlog::level::info;
        }
    }

    // LogLevel을 문자열로 변환
    const char* LogLevelToString(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Always: return "ALWAYS";
            default: return "UNKNOWN";
        }
    }
}

void InitLogger(const LoggerConfig& config)
{
    try
    {
        // 로그 디렉토리 생성
        if (config.enableFile && !config.logDirectory.empty())
        {
            std::filesystem::create_directories(config.logDirectory);
        }

        // thread pool 초기화 (async 로깅용)
        spdlog::init_thread_pool(config.asyncQueueSize, config.threadPoolSize);

        // sinks 생성
        std::vector<spdlog::sink_ptr> sinks;

        // 콘솔 sink (색상 지원)
        if (config.enableConsole)
        {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);

            // 색상 설정
            console_sink->set_color(spdlog::level::debug, console_sink->white);
            console_sink->set_color(spdlog::level::info, console_sink->green);
            console_sink->set_color(spdlog::level::err, console_sink->red);
            console_sink->set_color(spdlog::level::critical, console_sink->cyan);

            sinks.push_back(console_sink);
        }

        // 파일 sink (날짜별)
        if (config.enableFile)
        {
            std::string log_path = config.logDirectory + "/" + config.logFilePrefix;
            auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                log_path, 0, 0);  // 자정(0:0)에 파일 로테이션
            file_sink->set_level(spdlog::level::trace);
            sinks.push_back(file_sink);
        }

        // async logger 생성
        g_logger = std::make_shared<spdlog::async_logger>(
            "game_logger",
            sinks.begin(),
            sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);

        // 로그 레벨 설정
        g_logger->set_level(spdlog::level::trace);

        // 패턴 설정: [시간][카테고리][중요도] | 메시지
        g_logger->set_pattern("[%H:%M:%S.%e]%v");

        // flush 정책: 1초마다
        g_logger->flush_every(std::chrono::seconds(1));

        // 전역 로거로 등록
        spdlog::set_default_logger(g_logger);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        // 로거 초기화 실패 시 stderr로 출력
        fprintf(stderr, "Log initialization failed: %s\n", ex.what());
    }
}

void SetLogLevel(LogLevel level)
{
    g_minLogLevel = level;
}

void Shutdown()
{
    if (g_logger)
    {
        g_logger->flush();
        spdlog::shutdown();
        g_logger.reset();
    }
}

void LogImpl(const char* category, LogLevel level, const std::string& message)
{
    // 레벨 필터링
    if (level < g_minLogLevel)
    {
        return;
    }

    if (!g_logger)
    {
        return;
    }

    // 메시지 포맷: [카테고리][중요도] | 메시지
    std::string formatted = fmt::format("[{}][{}] | {}", category, LogLevelToString(level), message);

    // spdlog로 로깅
    spdlog::level::level_enum spdlog_level = ToSpdlogLevel(level);
    g_logger->log(spdlog_level, formatted);
}

void Log(const char* category, LogLevel level, const char* message)
{
    LogImpl(category, level, message);
}

} // namespace base
