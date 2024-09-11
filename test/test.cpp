#include <gtest/gtest.h>
#include <vector>
#include <string>

int SlowGetPage(int key) {
    return key;
}

int SlowGetPageString(std::string key) {
    return key.length();
}

// std::vector<int> GetExpectedCache()

TEST(LRU_CACHE, base_1) {
    EXPECT_EQ(1, 2);
}

TEST(LRU_CACHE, base_2) {
    EXPECT_EQ('b', 'j');
}

TEST(LRU_CACHE, lenght_1) {
    EXPECT_EQ(1, 2);
}

TEST(LRU_CACHE, lenght_2) {
    EXPECT_EQ('b', 'j');
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}