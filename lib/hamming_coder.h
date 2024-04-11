#pragma once

#include <vector>
#include <cinttypes>

const int kBitsInByte = 8;
const int kExponentOffset = 23;

void print_vector(std::vector<uint8_t> vec);

class HammingCoder {
public:
    static int CeiledLog2(int a);
    static int CeiledLog10(int a);
    static int CeilDivide(int a, int b);
    static bool IsPowerOfTwo(int a);
    static int CommonIndexToRedundant(int a);
    static int RedundantIndexToCommon(int a);
    static int GetAmountOfControlBits(int a);
    static int GetSizeOfEncodedBlock(int a);

    bool are_there_two_errors = false;
    bool is_one_error_corrected = false;

    HammingCoder() = default;
    static std::vector<uint8_t> EncodeBlock(std::vector<uint8_t> block);
    std::vector<uint8_t> DecodeBlock(std::vector<uint8_t> block);
};