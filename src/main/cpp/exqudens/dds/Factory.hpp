#pragma once

#include <memory>

#include "exqudens/dds/IWriter.hpp"
#include "exqudens/dds/IReader.hpp"

namespace exqudens::dds {

    class EXQUDENS_DDS_EXPORT Factory {

        private:

            inline static std::function<void(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                unsigned short level,
                const std::string& message
            )> logFunction = {};

        public:

            Factory() = delete;

            ~Factory() = delete;

            static void setLogFunction(
                const std::function<void(
                    const std::string& file,
                    size_t line,
                    const std::string& function,
                    const std::string& id,
                    unsigned short level,
                    const std::string& message
                )>& value
            );

            static std::string getVersion();

            static std::shared_ptr<IWriter> createSharedWriter();

            static std::shared_ptr<IReader> createSharedReader();

        private:

            static void log(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                unsigned short level,
                const std::string& message
            );

            static void logDebug(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                const std::string& message
            );

    };

}
