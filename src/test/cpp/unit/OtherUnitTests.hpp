#pragma once

#include <string>
#include <exception>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/Log.hpp>
#include <exqudens/log/api/Logging.hpp>

#include "TestUtils.hpp"
#include "exqudens/dds/Factory.hpp"

namespace exqudens::dds {

    class OtherUnitTests: public testing::Test {

        public:

            inline static const char* LOGGER_ID = "OtherUnitTests";

    };

    TEST_F(OtherUnitTests, test1) {
        try {
            std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
            std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
            EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

            std::string value = {};
            std::optional<std::vector<uint8_t>> data = {};
            std::shared_ptr<IWriter> writer = {};

            value = "Abc123!";
            data = std::vector<uint8_t>(value.begin(), value.end());
            writer = Factory::createSharedWriter();
            writer->open();

            uint32_t lastId = 1;
            while (1) {
                writer->write(data, lastId);
                lastId++;
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

    TEST_F(OtherUnitTests, test2) {
        try {
            std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
            std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
            EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

            std::shared_ptr<IReader> reader = {};
            std::optional<std::vector<uint8_t>> data = {};
            std::string value = {};

            reader = Factory::createSharedReader();
            reader->open();

            data = reader->read();
            while (!data) {
                data = reader->read();
            }
            value = std::string(data.value().begin(), data.value().end());
            EXQUDENS_LOG_INFO(LOGGER_ID) << "value: '" << value << "'";

            reader->close();
            reader.reset();

            EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
        } catch (const std::exception& e) {
            std::string errorMessage = TestUtils::toString(e);
            std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
            FAIL() << errorMessage;
        }
    }

}
