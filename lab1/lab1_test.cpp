#include <gtest/gtest.h>

#include "utils.h"
#include "parent.h"

#define PATH_TO_CHILD1 ("C:\\Users\\mashu\\OneDrive\\Документы\\маи\\3 сем\\os\\OS\\lab1\\src\\child1.cpp")
#define PATH_TO_CHILD2 ("C:\\Users\\mashu\\OneDrive\\Документы\\маи\\3 сем\\os\\OS\\lab1\\src\\child2.cpp")

void TestParent(std::string &src, std::string &res) {
    std::istringstream srcStream(src);
    std::streambuf* buf = std::cin.rdbuf(srcStream.rdbuf());

    testing::internal::CaptureStdout();
    InitParent(PATH_TO_CHILD1, PATH_TO_CHILD2);
    ASSERT_EQ(testing::internal::GetCapturedStdout(), res + '\n'); 

    std::cin.rdbuf(buf);
}

TEST (input_test, SimpleInput_1) {
    std::string input = "asdf";
    std::string output = "ASDF";
    TestParent(input, output);
}

TEST (input_test, SimpleInput_2) {
    std::string input = "asdf asdf";
    std::string output = "ASDF ASDF";
    TestParent(input, output);
}

TEST (input_test, SimpleInput_3) {
    std::string input = "asdf        asdf";
    std::string output = "ASDF ASDF";
    TestParent(input, output);
}

TEST (input_test, VoidInput) {
    std::string input = "";
    std::string output = "";
    TestParent(input, output);
}

TEST (input_test, OneSpaceInput) {
    std::string input = " ";
    std::string output = " ";
    TestParent(input, output);
}

TEST (input_test, SpacesInput) {
    std::string input = "         ";
    std::string output = " ";
    TestParent(input, output);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}