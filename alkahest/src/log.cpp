#include <alkahest/log.h>

namespace Alkahest
{
    void logTrace(const std::string& message, const bool color)
    {
        log(message, log_level::TRACE, color);
    }

    void logDebug(const std::string& message, const bool color)
    {
        log(message, log_level::DEBUG, color);
    }

    void logInfo(const std::string& message, const bool color)
    {
        log(message, log_level::INFO, color);
    }

    void logWarning(const std::string& message, const bool color)
    {
        log(message, log_level::WARN, color);
    }

    void logError(const std::string& message, const bool color)
    {
        log(message, log_level::ERROR, color);
    }
}
