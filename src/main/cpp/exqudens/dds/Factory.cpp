#include <filesystem>
#include <stdexcept>

#include "exqudens/dds/Factory.hpp"
#include "exqudens/dds/versions.hpp"
#include "exqudens/dds/Writer.hpp"
#include "exqudens/dds/Reader.hpp"

#define CALL_INFO std::string(__FUNCTION__) + " (" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"
#define LOGGER_ID "exqudens.dds.Factory"

namespace exqudens::dds {

    void Factory::setLogFunction(
        const std::function<void(
            const std::string& file,
            size_t line,
            const std::string& function,
            const std::string& id,
            unsigned short level,
            const std::string& message
        )>& value
    ) {
        try {
            logFunction = value;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::string Factory::getVersion() {
        try {
            std::string result = {};
            result += std::to_string(PROJECT_VERSION_MAJOR);
            result += '.';
            result += std::to_string(PROJECT_VERSION_MINOR);
            result += '.';
            result += std::to_string(PROJECT_VERSION_PATCH);
            logDebug(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, std::string("result: '") + result + "'");
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::shared_ptr<IWriter> Factory::createSharedWriter() {
        try {
            std::shared_ptr<IWriter> result = {};
            result = std::shared_ptr<IWriter>(new Writer);
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::shared_ptr<IReader> Factory::createSharedReader() {
        try {
            std::shared_ptr<IReader> result = {};
            result = std::shared_ptr<IReader>(new Reader);
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Factory::log(
        const std::string& file,
        size_t line,
        const std::string& function,
        const std::string& id,
        unsigned short level,
        const std::string& message
    ) {
        try {
            if (logFunction) {
                std::string internalFile = std::filesystem::path(file).filename().string();
                logFunction(internalFile, line, function, id, level, message);
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Factory::logDebug(
        const std::string& file,
        size_t line,
        const std::string& function,
        const std::string& id,
        const std::string& message
    ) {
        try {
            log(file, line, function, id, 5, message);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

}

#undef LOGGER_ID
#undef CALL_INFO
