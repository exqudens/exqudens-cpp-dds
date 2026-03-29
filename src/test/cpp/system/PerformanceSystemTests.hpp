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
#include <exqudens/log/api/Logging.hpp>
#include <exqudens/ThreadPool.hpp>

#include "TestUtils.hpp"
#include "exqudens/dds/Factory.hpp"

#define CALL_INFO std::string(__FUNCTION__) + " (" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"

class PerformanceSystemTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "PerformanceSystemTests";

    protected:

        static bool shallContinue(
            std::map<size_t, size_t>& storage,
            const std::optional<std::chrono::system_clock::time_point>& bgnTimePoint,
            size_t maxSeconds,
            bool store
        ) {
            try {
                std::chrono::system_clock::duration diff = {};
                std::chrono::seconds secondsDiff = {};
                bool result = true;
                if (bgnTimePoint) {
                    diff = std::chrono::system_clock::now() - bgnTimePoint.value();
                    secondsDiff = std::chrono::duration_cast<std::chrono::seconds>(diff);
                    result = secondsDiff.count() < maxSeconds;
                    if (result && !storage.empty() && store) {
                        std::chrono::milliseconds milliDiff = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
                        size_t key = milliDiff.count() >= 0 ? milliDiff.count() : std::numeric_limits<size_t>::max();
                        if (storage.contains(key)) {
                            storage.at(key) = storage.at(key) + 1;
                        } else {
                            storage.at(std::numeric_limits<size_t>::max()) = storage.at(std::numeric_limits<size_t>::max()) + 1;
                        }
                    }
                }
                return secondsDiff.count() < maxSeconds;
            } catch (...) {
                std::throw_with_nested(std::runtime_error(CALL_INFO));
            }
        }

        static std::map<size_t, size_t> createMilliSecondsCountMap(
            size_t maxSeconds,
            size_t factor = 1000
        ) {
            try {
                std::map<size_t, size_t> result = {};
                size_t size = maxSeconds * factor;
                for (size_t i = 0; i < size; i++) {
                    size_t key = i;
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

        TestUtils::windowsOnlyCurrentProcessSetPriorityClass();
        TestUtils::windowsOnlyTimeBeginPeriod();

        size_t maxSeconds = TestUtils::getTimeout().value_or(15);
        std::map<size_t, size_t> storage = {};
        uint32_t lastId = 1;
        size_t lastValue = 1;
        std::shared_ptr<exqudens::dds::IWriter> writer = exqudens::dds::Factory::createSharedWriter();
        std::optional<std::chrono::system_clock::time_point> bgnTimePoint = std::chrono::system_clock::now();

        writer->open();

        while (shallContinue(storage, bgnTimePoint, maxSeconds, false)) {
            std::vector<uint8_t> data = TestUtils::sizeToBytes(lastValue);
            writer->write(data, lastId);

            lastId++;
            lastValue++;
        }

        writer->close();
        writer.reset();

        TestUtils::windowsOnlyTimeEndPeriod();

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

        TestUtils::windowsOnlyCurrentProcessSetPriorityClass();
        TestUtils::windowsOnlyTimeBeginPeriod();

        size_t maxSeconds = TestUtils::getTimeout().value_or(5);
        std::map<size_t, size_t> storage = createMilliSecondsCountMap(maxSeconds);
        std::optional<std::chrono::system_clock::time_point> bgnTimePoint = {};
        std::shared_ptr<exqudens::dds::IReader> reader = exqudens::dds::Factory::createSharedReader();
        size_t lastValue = 0;
        std::optional<std::vector<uint8_t>> data = {};

        reader->open();

        while (shallContinue(storage, bgnTimePoint, maxSeconds, data.has_value())) {
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

        TestUtils::windowsOnlyTimeEndPeriod();

        std::vector<std::string> errorLines = {};
        for (const std::pair<size_t, size_t>& pair : storage) {
            if (pair.first == std::numeric_limits<size_t>::max()) {
                if (pair.second != 0) {
                    std::string errorLine = {};
                    errorLine += std::to_string(pair.first);
                    errorLine += ": ";
                    errorLine += std::to_string(pair.second);
                    errorLines.emplace_back(errorLine);
                }
            } else {
                if (pair.second == 0) {
                    std::string errorLine = {};
                    errorLine += std::to_string(pair.first);
                    errorLine += ": ";
                    errorLine += std::to_string(pair.second);
                    errorLines.emplace_back(errorLine);
                }
            }
        }

        if (!errorLines.empty()) {
            FAIL() << TestUtils::join(errorLines, TestUtils::lineSeparator());
        }

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(PerformanceSystemTests, test3) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        exqudens::ThreadPool threadPool = {};
        threadPool.start(2, 2);

        std::string commandLine1 = TestUtils::join({
            TestUtils::getExecutableFile().value(),
            "--gtest_filter=PerformanceSystemTests.test1"
        }, " ");
        EXQUDENS_LOG_INFO(LOGGER_ID) << "commandLine1: '" << commandLine1 << "'";
        std::string commandLine2 = TestUtils::join({
            TestUtils::getExecutableFile().value(),
            "--gtest_filter=PerformanceSystemTests.test2"
        }, " ");
        EXQUDENS_LOG_INFO(LOGGER_ID) << "commandLine2: '" << commandLine2 << "'";

        std::future<std::pair<int, std::string>> future1 = threadPool.submit(&TestUtils::executeCommandLine, commandLine1);
        std::future<std::pair<int, std::string>> future2 = threadPool.submit(&TestUtils::executeCommandLine, commandLine2);

        std::pair<int, std::string> result1 = future1.get();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "result1.first: " << result1.first;
        EXQUDENS_LOG_INFO(LOGGER_ID) << "result1.second: '" << result1.second << "'";
        std::pair<int, std::string> result2 = future2.get();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "result2.first: " << result2.first;
        EXQUDENS_LOG_INFO(LOGGER_ID) << "result2.second: '" << result2.second << "'";

        threadPool.stop();
        ASSERT_FALSE(threadPool.isStarted());

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

#undef CALL_INFO
