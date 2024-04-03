#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#define TEST_LOGGER "gtest_logger"
auto LOG_LEVEL = spdlog::level::debug;

class GlobalSetup : public ::testing::Environment {
public:
    void SetUp() override {
        spdlog::set_level(LOG_LEVEL);
    }

    void TearDown() override {
       
    }
};