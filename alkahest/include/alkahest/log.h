#pragma once

#include <fmt/format.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <chrono>
#include <ctime>

namespace Alkahest
{
    enum class log_level : std::uint8_t
    {
        TRACE = 0,
        DEBUG = 1,
        INFO  = 2,
        WARN  = 3,
        ERROR = 4
    };

    struct enum_hasher
    {
        template<typename T> std::size_t operator()(T t) const { return static_cast<std::size_t>(t); };
    };

    const std::unordered_map<log_level, std::string, enum_hasher> uncoloredText
    {
        { log_level::TRACE, " [TRACE] "   },
        { log_level::DEBUG, " [DEBUG] "   },
        { log_level::INFO , " [INFO] "    },
        { log_level::WARN , " [WARNING] " },
        { log_level::ERROR, " [ERROR] "   }
    };

    const std::unordered_map<log_level, std::string, enum_hasher> coloredText
    {
        { log_level::TRACE, " \x1b[37;1m[TRACE]\x1b[0m "   },
        { log_level::DEBUG, " \x1b[34;1m[DEBUG]\x1b[0m "   },
        { log_level::INFO , " \x1b[32;1m[INFO]\x1b[0m "    },
        { log_level::WARN , " \x1b[33;1m[WARNING]\x1b[0m " },
        { log_level::ERROR, " \x1b[31;1m[ERROR]\x1b[0m "   }        
    };

    #if defined(LOGGING_LEVEL_ALL) || defined(LOGGING_LEVEL_TRACE)
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::TRACE;
    #elif defined(LOGGING_LEVEL_DEBUG)
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::DEBUG;
    #elif defined(LOGGING_LEVEL_WARN)
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::WARN;
    #elif defined(LOGGING_LEVEL_ERROR)
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::ERROR;
    #elif defined(LOGGING_LEVEL_NONE)
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::ERROR + 1;
    #else
        constexpr log_level LOG_LEVEL_CUTOFF = log_level::INFO;
    #endif

    namespace {
        std::string timestamp()
        {
            std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
            std::time_t tt = std::chrono::system_clock::to_time_t(tp);
            std::tm gmt{}; gmtime_r(&tt, &gmt);
            std::chrono::duration<double> fractional_seconds =
                (tp - std::chrono::system_clock::from_time_t(tt)) + std::chrono::seconds(gmt.tm_sec);
            std::string buffer("YYYY-MM-DDThh:mm:ss.xxxxxx");
            sprintf(&buffer.front(), "%04d-%02d-%02dT%02d:%02d:%09.6f", gmt.tm_year + 1900, gmt.tm_mon + 1,
                gmt.tm_mday, gmt.tm_hour, gmt.tm_min, fractional_seconds.count());
            return buffer;
        }

        void log(const std::string& message)
        {
            std::cout << message;
            std::cout.flush();
        }

        void log(const std::string& message, const log_level level, bool color)
        {
            if (level < LOG_LEVEL_CUTOFF)
            {
                return;
            }

            std::string output;
            std::unordered_map<log_level, std::string, enum_hasher> levels =
                color ? coloredText : uncoloredText;
            output.reserve(message.length() + 64);
            output.append(timestamp());
            output.append(levels.find(level)->second);
            output.append(message);
            output.push_back('\n');
            log(output);
        }
    }

    template<typename... Args>
    void logTrace(const std::string& fmtStr, Args&&... args)
    {
        std::string message = fmt::vformat(fmtStr, fmt::make_format_args(args...));
        log(message, log_level::TRACE, true);
    }

    template<typename... Args>
    void logDebug(const std::string& fmtStr, Args&&... args)
    {
        std::string message = fmt::vformat(fmtStr, fmt::make_format_args(args...));
        log(message, log_level::DEBUG, true);
    }

    template<typename... Args>
    void logInfo(const std::string& fmtStr, Args&&... args)
    {
        std::string message = fmt::vformat(fmtStr, fmt::make_format_args(args...));
        log(message, log_level::INFO, true);
    }

    template<typename... Args>
    void logWarning(const std::string& fmtStr, Args&&... args)
    {
        std::string message = fmt::vformat(fmtStr, fmt::make_format_args(args...));
        log(message, log_level::WARN, true);
    }

    template<typename... Args>
    void logError(const std::string& fmtStr, Args&&... args)
    {
        std::string message = fmt::vformat(fmtStr, fmt::make_format_args(args...));
        log(message, log_level::ERROR, true);
    }


    void logTrace(const std::string& message, const bool color = true);
    void logDebug(const std::string& message, const bool color = true);
    void logInfo(const std::string& message, const bool color = true);
    void logWarning(const std::string& message, const bool color = true);
    void logError(const std::string& message, const bool color = true);
}
