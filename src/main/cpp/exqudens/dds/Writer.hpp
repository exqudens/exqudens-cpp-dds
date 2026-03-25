#pragma once

#include <memory>

#include <dds/dds.hpp>

#include "exqudens/dds/IWriter.hpp"
#include "exqudens/dds/model/BytesMessage.hpp"

namespace exqudens::dds {

    class EXQUDENS_DDS_EXPORT Writer : public IWriter {

        private:

            std::function<void(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                unsigned short level,
                const std::string& message
            )> logFunction = {};
            bool autoClose = false;
            std::shared_ptr<::dds::domain::DomainParticipant> participant = {};
            std::shared_ptr<::dds::topic::Topic<model::BytesMessage>> topic = {};
            std::shared_ptr<::dds::pub::Publisher> publisher = {};
            std::shared_ptr<::dds::pub::DataWriter<model::BytesMessage>> writer = {};

        public:

            Writer(bool autoClose);
            Writer();

            void setLogFunction(
                const std::function<void(
                    const std::string& file,
                    size_t line,
                    const std::string& function,
                    const std::string& id,
                    unsigned short level,
                    const std::string& message
                )>& value
            ) override;

            bool isSetLogFunction() override;

            void open(
                const std::optional<std::string>& topicName,
                const std::optional<uint32_t>& domainParticipantId
            ) override;
            void open(
                const std::optional<std::string>& topicName
            ) override;
            void open() override;

            bool isOpen() override;

            void write(const std::optional<std::vector<uint8_t>>& value, const std::optional<uint32_t>& id) override;
            void write(const std::optional<std::vector<uint8_t>>& value) override;

            void close() override;

            ~Writer() override;

        private:

            void log(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                unsigned short level,
                const std::string& message
            );

            void logWarning(
                const std::string& file,
                size_t line,
                const std::string& function,
                const std::string& id,
                const std::string& message
            );

    };

}
