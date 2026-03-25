#pragma once

#include <memory>

#include <dds/dds.hpp>

#include "exqudens/dds/IReader.hpp"
#include "exqudens/dds/model/BytesMessage.hpp"

namespace exqudens::dds {

    class EXQUDENS_DDS_EXPORT Reader : public IReader {

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
            std::shared_ptr<::dds::sub::Subscriber> subscriber = {};
            std::shared_ptr<::dds::sub::DataReader<model::BytesMessage>> reader = {};

        public:

            Reader(bool autoClose);
            Reader();

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

            std::optional<std::vector<uint8_t>> read() override;

            void close() override;

            ~Reader() override;

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
