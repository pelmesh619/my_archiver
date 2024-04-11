#include <tuple>
#include <cstring>

#include <gtest/gtest.h>

#include "lib/hamming_coder.h"


TEST(Log2Test, Test1) {
    int number = 2;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 1);
}
TEST(Log2Test, Test2) {
    int number = 3;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 2);
}
TEST(Log2Test, Test3) {
    int number = 4;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 2);
}
TEST(Log2Test, Test4) {
    int number = 5;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 3);
}
TEST(Log2Test, Test5) {
    int number = 6;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 3);
}
TEST(Log2Test, Test6) {
    int number = 8;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 3);
}
TEST(Log2Test, Test7) {
    int number = 9;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 4);
}
TEST(Log2Test, Test8) {
    int number = 1;
    int result = HammingCoder::CeiledLog2(number);
    ASSERT_EQ(result, 0);
}


TEST(IsPowerOfTwoTest, Test1) {
    int number = 2;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, true);
}
TEST(IsPowerOfTwoTest, Test2) {
    int number = 3;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, false);
}
TEST(IsPowerOfTwoTest, Test3) {
    int number = 4;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, true);
}
TEST(IsPowerOfTwoTest, Test4) {
    int number = 5;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, false);
}
TEST(IsPowerOfTwoTest, Test5) {
    int number = 6;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, false);
}
TEST(IsPowerOfTwoTest, Test6) {
    int number = 7;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, false);
}
TEST(IsPowerOfTwoTest, Test7) {
    int number = 8;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, true);
}
TEST(IsPowerOfTwoTest, Test8) {
    int number = 1;
    bool result = HammingCoder::IsPowerOfTwo(number);
    ASSERT_EQ(result, true);
}

TEST(IndexConverseTest, Test1) {
    int number = 1;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 3);
    ASSERT_EQ(result2, number);
}

TEST(IndexConverseTest, Test2) {
    int number = 2;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 5);
    ASSERT_EQ(result2, number);
}

TEST(IndexConverseTest, Test3) {
    int number = 4;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 7);
    ASSERT_EQ(result2, number);
}

TEST(IndexConverseTest, Test4) {
    int number = 5;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 9);
    ASSERT_EQ(result2, number);
}

TEST(IndexConverseTest, Test5) {
    int number = 11;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 15);
    ASSERT_EQ(result2, number);
}

TEST(IndexConverseTest, Test6) {
    int number = 12;
    int result = HammingCoder::CommonIndexToRedundant(number);
    int result2 = HammingCoder::RedundantIndexToCommon(result);
    ASSERT_EQ(result, 17);
    ASSERT_EQ(result2, number);
}

TEST(HammingEncodeTest, Test1) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10001101, 0b10101011};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    std::vector<uint8_t> answer = {0b10001101, 0b10101011, 0b111011};
    ASSERT_EQ(result, answer);
}

TEST(HammingEncodeTest, Test2) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b01010010, 0b10010101};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    std::vector<uint8_t> answer = {0b01010010, 0b10010101, 0b100110};
    ASSERT_EQ(result, answer);
}

TEST(HammingDecodeTest, Test1) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10001101, 0b10101011, 0b111011};
    std::vector<uint8_t> result = coder.DecodeBlock(message);
    std::vector<uint8_t> answer = {0b10001101, 0b10101011};
    ASSERT_EQ(result, answer);
}

TEST(HammingDecodeTest, Test2) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10001101, 0b10101011};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[1] ^= 1 << 4;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test3) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10111011, 0b10110110, 0b10110100, 0b10111110};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[4] ^= 1 << 3;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test4) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10111011, 0b10110110, 0b10110100, 0b10111110};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[2] ^= 1 << 6;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test5) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b00110000, 0b01110110, 0b10101011, 0b00100001, 0b00100000};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[0] ^= 1 << 4;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[3] ^= 1 << 1;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test6) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            0b00011011, 0b10010001, 0b11111010, 0b01111111,
            0b10110001, 0b01100010, 0b01110111, 0b11110011,
            0b00010110, 0b00010101, 0b00101000, 0b00110111,
            0b01001011, 0b01000000, 0b11001011
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[0] ^= 1 << 4;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[4] ^= 1 << 7;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[8] ^= 1 << 6;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[11] ^= 1 << 0;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[11] ^= 1 << 4;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[3] ^= 1 << 3;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[5] ^= 1 << 1;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[8] ^= 1 << 5;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[10] ^= 1 << 3;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[16] ^= 1 << 3;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test7) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            0b01101011, 0b10001101, 0b11000010, 0b11100001,
            0b01011001, 0b01001011, 0b11010010, 0b10101011,
            0b10111111, 0b00110000, 0b11101010, 0b00111111,
            0b10000010, 0b11110110, 0b00101111, 0b01110000,
            0b00111110, 0b10100110, 0b00011111, 0b10100010,
            0b00100111, 0b00100111, 0b10101101, 0b00000110,
            0b10010001, 0b00110100, 0b00101110, 0b10011101,
            0b00110100, 0b10110010, 0b10110011, 0b10000000
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[0] ^= 1 << 4;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[33] ^= 1 << 4;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test8) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            0b01011001, 0b10100100, 0b11011111, 0b01101010,
            0b11001110, 0b11100011, 0b00111001, 0b01011101
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[2] ^= 1 << 7;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[6] ^= 1 << 2;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingDecodeTest, Test9) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            232, 183, 231, 8, 217, 249, 229, 199,
            116, 100, 130, 45, 0, 12, 29, 163,
            20, 234, 139, 102, 11, 120, 35, 175,
            223, 77, 67, 189, 17, 57, 176, 157,
            69, 173, 65, 36, 108, 218, 186, 99,
            154, 231, 11, 218, 186, 116, 151, 25,
            83, 211, 86, 143, 165, 140, 207, 45,
            103, 48, 105, 166, 234, 3, 52, 50,
            140, 27, 181, 234, 177, 32, 230, 250,
            91, 5, 11, 5, 108, 70, 50, 61,
            9, 212, 104, 96, 61, 250, 25, 217,
            21, 113, 228, 185, 24, 36, 210, 219,
            81, 79, 15, 254, 241, 133, 117, 100,
            107, 183, 186, 44, 71, 121, 146, 130,
            90, 59, 154, 215, 155, 207, 73, 9,
            42, 23, 65, 117, 195, 129, 74, 163
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[158] ^= 1 << 7;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);

    result = coder.EncodeBlock(message);
    result[23] ^= 1 << 0;
    answer = coder.DecodeBlock(result);

    ASSERT_EQ(answer, message);
}

TEST(HammingTwoErrorTest, Test1) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10111011, 0b10110110, 0b10110100, 0b10111110};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, false);
}

TEST(HammingTwoErrorTest, Test2) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10111011, 0b10110110, 0b10110100, 0b10111110};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[2] ^= 1 << 7;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, false);
}

TEST(HammingTwoErrorTest, Test3) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b10111011, 0b10110110, 0b10110100, 0b10111110};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[2] ^= 1 << 7;
    result[1] ^= 1 << 7;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, true);
}

TEST(HammingTwoErrorTest, Test4) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            232, 183, 231, 8, 217, 249, 229, 199,
            116, 100, 130, 45, 0, 12, 29, 163,
            20, 234, 139, 102, 11, 120, 35, 175,
            223, 77, 67, 189, 17, 57, 176, 157,
            69, 173, 65, 36, 108, 218, 186, 99,
            154, 231, 11, 218, 186, 116, 151, 25,
            83, 211, 86, 143, 165, 140, 207, 45,
            103, 48, 105, 166, 234, 3, 52, 50,
            140, 27, 181, 234, 177, 32, 230, 250,
            91, 5, 11, 5, 108, 70, 50, 61,
            9, 212, 104, 96, 61, 250, 25, 217,
            21, 113, 228, 185, 24, 36, 210, 219,
            81, 79, 15, 254, 241, 133, 117, 100,
            107, 183, 186, 44, 71, 121, 146, 130,
            90, 59, 154, 215, 155, 207, 73, 9,
            42, 23, 65, 117, 195, 129, 74, 163
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[13] ^= 1 << 2;
    result[67] ^= 1 << 5;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, true);
}

TEST(HammingTwoErrorTest, Test5) {
    HammingCoder coder;
    std::vector<uint8_t> message = {
            232, 183, 231, 8, 217, 249, 229, 199,
            116, 100, 130, 45, 0, 12, 29, 163,
            20, 234, 139, 102, 11, 120, 35, 175,
            223, 77, 67, 189, 17, 57, 176, 157,
            69, 173, 65, 36, 108, 218, 186, 99,
            154, 231, 11, 218, 186, 116, 151, 25,
            83, 211, 86, 143, 165, 140, 207, 45,
            103, 48, 105, 166, 234, 3, 52, 50,
            140, 27, 181, 234, 177, 32, 230, 250,
            91, 5, 11, 5, 108, 70, 50, 61,
            9, 212, 104, 96, 61, 250, 25, 217,
            21, 113, 228, 185, 24, 36, 210, 219,
            81, 79, 15, 254, 241, 133, 117, 100,
            107, 183, 186, 44, 71, 121, 146, 130,
            90, 59, 154, 215, 155, 207, 73, 9,
            42, 23, 65, 117, 195, 129, 74, 163
    };
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[104] ^= 1 << 2;
    result[12] ^= 1 << 5;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, true);
}

TEST(HammingTwoErrorTest, Test6) {
    HammingCoder coder;
    std::vector<uint8_t> message = {0b01011001, 0b10100000};
    std::vector<uint8_t> result = coder.EncodeBlock(message);
    result[0] ^= 1 << 6;
    result[0] ^= 1 << 5;
    std::vector<uint8_t> answer = coder.DecodeBlock(result);

    ASSERT_EQ(coder.are_there_two_errors, true);
}