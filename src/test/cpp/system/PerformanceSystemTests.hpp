#pragma once

#include <string>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <optional>
#include <chrono>
#include <map>
#include <cstddef>
#include <cstdint>
#include <limits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/Log.hpp>
#include <exqudens/log/api/Logging.hpp>

#include "TestUtils.hpp"
#include "exqudens/dds/Factory.hpp"

#define CALL_INFO std::string(__FUNCTION__) + " (" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

class PerformanceSystemTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "PerformanceSystemTests";
        inline static const size_t FACTOR = 1000;

    protected:

        static bool shallContinue(
            std::map<size_t, size_t>& microSecondsCountMap,
            const std::optional<std::chrono::system_clock::time_point>& bgnTimePoint,
            size_t maxSeconds,
            bool store,
            size_t factor = FACTOR
        ) {
            try {
                std::chrono::system_clock::duration diff = {};
                std::chrono::seconds secondsDiff = {};
                bool result = true;
                if (bgnTimePoint) {
                    diff = std::chrono::system_clock::now() - bgnTimePoint.value();
                    secondsDiff = std::chrono::duration_cast<std::chrono::seconds>(diff);
                    result = secondsDiff.count() < maxSeconds;
                    if (result && !microSecondsCountMap.empty() && store) {
                        std::chrono::microseconds microDiff = std::chrono::duration_cast<std::chrono::microseconds>(diff);
                        size_t key = (size_t) microDiff.count() / factor;
                        key = factor * key;
                        if (microSecondsCountMap.contains(key)) {
                            microSecondsCountMap.at(key) = microSecondsCountMap.at(key) + 1;
                        } else {
                            microSecondsCountMap.at(std::numeric_limits<size_t>::max()) = microSecondsCountMap.at(std::numeric_limits<size_t>::max()) + 1;
                        }
                    }
                }
                return secondsDiff.count() < maxSeconds;
            } catch (...) {
                std::throw_with_nested(std::runtime_error(CALL_INFO));
            }
        }

        static std::map<size_t, size_t> createMicroSecondsCountMap(
            size_t x,
            size_t y = 1000,
            size_t factor = FACTOR
        ) {
            try {
                std::map<size_t, size_t> result = {};
                size_t size = x * y;
                for (size_t i = 0; i < size; i++) {
                    size_t key = i;
                    key *= factor;
                    result[key] = 0;
                }
                result[std::numeric_limits<size_t>::max()] = 0;
                return result;
            } catch (...) {
                std::throw_with_nested(std::runtime_error(CALL_INFO));
            }
        }

};

TEST_F(PerformanceSystemTests, test1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        size_t maxSeconds = 15;
        std::map<size_t, size_t> microSecondsCountMap = {};
        uint32_t lastId = 1;
        size_t lastValue = 1;
        std::shared_ptr<exqudens::dds::IWriter> writer = exqudens::dds::Factory::createSharedWriter();
        std::optional<std::chrono::system_clock::time_point> bgnTimePoint = std::chrono::system_clock::now();

        writer->open();

        while (shallContinue(microSecondsCountMap, bgnTimePoint, maxSeconds, false)) {
            std::vector<uint8_t> data = TestUtils::sizeToBytes(lastValue);
            writer->write(data, lastId);

            lastId++;
            lastValue++;
        }

        writer->close();
        writer.reset();

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(PerformanceSystemTests, test2) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        size_t maxSeconds = 5;
        std::map<size_t, size_t> microSecondsCountMap = createMicroSecondsCountMap(maxSeconds);
        std::optional<std::chrono::system_clock::time_point> bgnTimePoint = {};
        std::shared_ptr<exqudens::dds::IReader> reader = exqudens::dds::Factory::createSharedReader();
        size_t lastValue = 0;
        std::optional<std::vector<uint8_t>> data = {};

        reader->open();

        while (shallContinue(microSecondsCountMap, bgnTimePoint, maxSeconds, data.has_value())) {
            data = reader->read();
            if (data) {
                if (!bgnTimePoint) {
                    bgnTimePoint = std::chrono::system_clock::now();
                }
                std::optional<size_t> optionalValue = TestUtils::bytesToSize(data.value());
                if (optionalValue.value() <= lastValue) {
                    FAIL() << "optionalValue.value() <= lastValue";
                }
                lastValue = optionalValue.value();
            }
        }

        reader->close();
        reader.reset();

        for (const auto& pair : microSecondsCountMap) EXQUDENS_LOG_INFO(LOGGER_ID) << "microSecondsCountMap[" << pair.first << "]: " << pair.second;

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(PerformanceSystemTests, test999) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        size_t maxSeconds = 2;
        std::map<size_t, size_t> microSecondsCountMap = createMicroSecondsCountMap(maxSeconds);
        for (const auto& pair : microSecondsCountMap) {
            EXQUDENS_LOG_INFO(LOGGER_ID) << "microSecondsCountMap[" << pair.first << "]: " << pair.second;
        }
        EXQUDENS_LOG_INFO(LOGGER_ID) << "microSecondsCountMap.size: " << microSecondsCountMap.size();

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

#undef CALL_INFO
