#ifndef APPLOGGER_H
#define APPLOGGER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "spdlog/spdlog.h"

#define LOG_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), __VA_ARGS__)

class AppLogger
{
public:
    enum { LOG_CONSOLE = 1, LOG_ROTATE, LOG_DAILY };
    AppLogger();
    ~AppLogger();

    static void init(int type);
};

#endif // APPLOGGER_H
