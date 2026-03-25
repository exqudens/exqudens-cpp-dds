#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include <functional>

#include "exqudens/dds/export.hpp"

namespace exqudens::dds {

    class EXQUDENS_DDS_EXPORT IWriter {

        public:

            virtual void setLogFunction(
                const std::function<void(
                    const std::string& file,
                    size_t line,
                    const std::string& function,
                    const std::string& id,
                    unsigned short level,
                    const std::string& message
                )>& value
            ) = 0;

            virtual bool isSetLogFunction() = 0;

            virtual void open(
                const std::optional<std::string>& topicName,
                const std::optional<uint32_t>& domainParticipantId
            ) = 0;
            virtual void open(
                const std::optional<std::string>& topicName
            ) = 0;
            virtual void open() = 0;

            virtual bool isOpen() = 0;

            virtual void write(const std::optional<std::vector<uint8_t>>& value, const std::optional<uint32_t>& id) = 0;
            virtual void write(const std::optional<std::vector<uint8_t>>& value) = 0;

            virtual void close() = 0;

            virtual ~IWriter() = default;

    };

}
