#include <filesystem>
#include <stdexcept>

#include "exqudens/dds/Reader.hpp"

#define CALL_INFO std::string(__FUNCTION__) + " (" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"
#define LOGGER_ID "exqudens.dds.Reader"

namespace exqudens::dds {

    Reader::Reader(
        bool autoClose
    ):
        autoClose(autoClose)
    {}

    Reader::Reader(): Reader::Reader(
        true
    ) {}

    void Reader::setLogFunction(
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

    bool Reader::isSetLogFunction() {
        try {
            return (bool) logFunction;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Reader::open(
        const std::optional<std::string>& topicName,
        const std::optional<uint32_t>& domainParticipantId
    ) {
        try {
            if (isOpen()) {
                throw std::runtime_error(CALL_INFO + ": already open");
            }
            participant = std::make_shared<::dds::domain::DomainParticipant>(domainParticipantId.value_or(org::eclipse::cyclonedds::domain::default_id()));
            topic = std::make_shared<::dds::topic::Topic<model::BytesMessage>>(*participant.get(), topicName.value_or("exqudens_dds_model_BytesMessage"));
            subscriber = std::make_shared<::dds::sub::Subscriber>(*participant.get());
            reader = std::make_shared<::dds::sub::DataReader<model::BytesMessage>>(*subscriber.get(), *topic.get());
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Reader::open(
        const std::optional<std::string>& topicName
    ) {
        try {
            open(topicName, {});
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Reader::open() {
        try {
            open({}, {});
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    bool Reader::isOpen() {
        try {
            if (reader || subscriber || topic || participant) {
                return true;
            } {
                return false;
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::optional<std::vector<uint8_t>> Reader::read() {
        try {
            if (!isOpen()) {
                throw std::runtime_error(CALL_INFO + ": not open");
            }
            std::optional<std::vector<uint8_t>> result = {};
            ::dds::sub::LoanedSamples<model::BytesMessage> samples = {};
            samples = reader->select().max_samples(1).state(::dds::sub::status::DataState::new_data()).take();
            if (samples.length() > 0) {
                ::dds::sub::LoanedSamples<model::BytesMessage>::const_iterator iterator;
                for (iterator = samples.begin(); iterator < samples.end(); iterator++) {
                    const model::BytesMessage& sample = iterator->data();
                    const ::dds::sub::SampleInfo& info = iterator->info();
                    if (info.valid()) {
                        result = sample.data();
                        break;
                    }
                }
            }
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Reader::close() {
        try {
            if (reader) {
                reader.reset();
            }
            if (subscriber) {
                subscriber.reset();
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

    Reader::~Reader() {
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

    void Reader::log(
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

    void Reader::logWarning(
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
