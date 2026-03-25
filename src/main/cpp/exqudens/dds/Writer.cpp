#include <filesystem>
#include <stdexcept>

#include "exqudens/dds/Writer.hpp"

#define CALL_INFO std::string(__FUNCTION__) + " (" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"
#define LOGGER_ID "exqudens.dds.Writer"

namespace exqudens::dds {

    Writer::Writer(
        bool autoClose
    ):
        autoClose(autoClose)
    {}

    Writer::Writer(): Writer::Writer(
        true
    ) {}

    void Writer::setLogFunction(
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

    bool Writer::isSetLogFunction() {
        try {
            return (bool) logFunction;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::open(
        const std::optional<std::string>& topicName,
        const std::optional<uint32_t>& domainParticipantId
    ) {
        try {
            if (isOpen()) {
                throw std::runtime_error(CALL_INFO + ": already open");
            }
            participant = std::make_shared<::dds::domain::DomainParticipant>(domainParticipantId.value_or(org::eclipse::cyclonedds::domain::default_id()));
            topic = std::make_shared<::dds::topic::Topic<model::BytesMessage>>(*participant.get(), topicName.value_or("exqudens_dds_model_BytesMessage"));
            publisher = std::make_shared<::dds::pub::Publisher>(*participant.get());
            writer = std::make_shared<::dds::pub::DataWriter<model::BytesMessage>>(*publisher.get(), *topic.get());
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::open(
        const std::optional<std::string>& topicName
    ) {
        try {
            open(topicName, {});
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::open() {
        try {
            open({}, {});
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    bool Writer::isOpen() {
        try {
            if (writer || publisher || topic || participant) {
                return true;
            } {
                return false;
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::write(const std::optional<std::vector<uint8_t>>& value, const std::optional<uint32_t>& id) {
        try {
            if (!isOpen()) {
                throw std::runtime_error(CALL_INFO + ": not open");
            }
            if (!value) {
                return;
            }
            model::BytesMessage sample(id.value_or(1), value.value());
            writer->write(sample);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::write(const std::optional<std::vector<uint8_t>>& value) {
        try {
            write(value, {});
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Writer::close() {
        try {
            if (writer) {
                writer.reset();
            }
            if (publisher) {
                publisher.reset();
            }
            if (topic) {
                topic.reset();
            }
            if (participant) {
                participant.reset();
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    Writer::~Writer() {
        try {
            if (autoClose) {
                close();
            }
        } catch (const std::exception& e) {
            logWarning(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, CALL_INFO + ": error on close: '" + e.what() + "'");
        } catch (...) {
            logWarning(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, CALL_INFO + ": unknown error on close");
        }
    }

    void Writer::log(
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

    void Writer::logWarning(
        const std::string& file,
        size_t line,
        const std::string& function,
        const std::string& id,
        const std::string& message
    ) {
        try {
            log(file, line, function, id, 3, message);
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

}

#undef LOGGER_ID
#undef CALL_INFO
