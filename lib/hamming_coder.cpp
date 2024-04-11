#include <iostream>
#include "hamming_coder.h"


int HammingCoder::CeiledLog2(int a) {
    auto b = static_cast<float>(a);
    int result = (*reinterpret_cast<int*>(&b) >> kExponentOffset) - 127;
    if (a == 1 << result) {
        return result;
    }
    return result + 1;
}

int HammingCoder::CeiledLog10(int a) {
    int counter = 0;
    while (a > 0) {
        a /= 10;
        counter++;
    }
    return counter;
}

bool HammingCoder::IsPowerOfTwo(int a) {
    auto b = static_cast<float>(a);
    a = *reinterpret_cast<int*>(&b);
    return (a & ((1 << kExponentOffset) - 1)) == 0;
}

int HammingCoder::CeilDivide(int a, int b) {
    return (a + b - 1) / b;
}

int HammingCoder::CommonIndexToRedundant(int a) {
    int b = CeiledLog2(a + 1);
    b = CeiledLog2(a + b + 1);
    return a + b;
}

int HammingCoder::RedundantIndexToCommon(int a) {
    return a - CeiledLog2(a);
}

std::vector<uint8_t> HammingCoder::EncodeBlock(std::vector<uint8_t> block) {
    int length = block.size() * kBitsInByte;
    int control_bits = GetAmountOfControlBits(block.size());

    std::vector<uint8_t> control_bitset;
    for (int i = 0; i < control_bits + 1; i += kBitsInByte) {
        control_bitset.push_back(0);
    }

    int whole_block_parity_bit = 0;
    for (int i = 1; i < 1 + length; i++) {
        int bit = block[(i - 1) / kBitsInByte] >> (kBitsInByte - (i - 1) % kBitsInByte - 1);
        bit &= 1;
        if (!bit) {
            continue;
        }
        whole_block_parity_bit ^= 1;

        int j = CommonIndexToRedundant(i);
        int counter = 1;
        while (j > 0) {
            int v = j & 1;
            if (v == 1) {
                control_bitset[counter / kBitsInByte] ^= 1 << (counter % kBitsInByte);
            }
            counter++;
            j >>= 1;
        }
    }
    for (int i = control_bitset.size() - 1; i >= 0; i--) {
        int j = control_bitset[i];
        if (i == 0) {
            j >>= 1;
        }
        while (j > 0) {
            whole_block_parity_bit ^= j & 1;
            j >>= 1;
        }
    }
    control_bitset[0] |= whole_block_parity_bit & 1;

    std::vector<uint8_t> new_block = block;
    new_block.insert(new_block.end(), control_bitset.begin(), control_bitset.end());

    return new_block;
}

std::vector<uint8_t> HammingCoder::DecodeBlock(std::vector<uint8_t> block) {
    int length = block.size() * kBitsInByte;
    int control_bits = CeilDivide(CeiledLog2(length + 1), kBitsInByte) * kBitsInByte;
    int decoded_length = length - control_bits;
    control_bits = CeiledLog2(decoded_length + 1);
    control_bits = CeiledLog2(decoded_length + control_bits + 1);

    std::vector<uint8_t> control_bitset;

    for (int i = 0; i < control_bits + 1; i += kBitsInByte) {
        control_bitset.push_back(0);
    }

    int whole_block_parity_bit = 0;
    for (int i = 1; i < 1 + decoded_length; i++) {
        int bit = block[(i - 1) / kBitsInByte] >> (kBitsInByte - (i - 1) % kBitsInByte - 1);
        bit &= 1;
        if (!bit) {
            continue;
        }
        whole_block_parity_bit ^= 1;

        int j = CommonIndexToRedundant(i);
        int counter = 1; // 0 is "whole block" parity bit
        while (j > 0) {
            int v = j & 1;
            if (v == 1) {
                control_bitset[counter / kBitsInByte] ^= 1 << counter % kBitsInByte;
            }
            counter++;
            j >>= 1;
        }
    }

    std::vector<uint8_t> decoded_block(block.begin(), block.end() - CeilDivide(control_bits + 1, kBitsInByte));
    std::vector<uint8_t> real_control_bitset(block.end() - CeilDivide(control_bits + 1, kBitsInByte), block.end());


    for (int i = real_control_bitset.size() - 1; i >= 0; i--) {
        int j = real_control_bitset[i];
        if (i == 0) {
            j >>= 1;
        }
        while (j > 0) {
            whole_block_parity_bit ^= j & 1;
            j >>= 1;
        }
    }

    int real_whole_block_parity_bit = real_control_bitset[0] & 1;

    int error_bit = 0;
    for (int i = real_control_bitset.size() - 1; i >= 0; i--) {
        error_bit <<= kBitsInByte;
        error_bit += real_control_bitset[i] ^ control_bitset[i];
    }
    error_bit >>= 1;

    is_one_error_corrected = false;
    if (error_bit == 0) {
        are_there_two_errors = real_whole_block_parity_bit != whole_block_parity_bit;
        return decoded_block;
    }
    are_there_two_errors = real_whole_block_parity_bit == whole_block_parity_bit;
    if (are_there_two_errors) {
        return decoded_block;
    }
    if (IsPowerOfTwo(error_bit)) {
        return decoded_block;
    }
    int index = RedundantIndexToCommon(error_bit) - 1;
    if (index >= decoded_length) {
        return decoded_block;
    }
    decoded_block[index / 8] ^= 1 << (kBitsInByte - index % kBitsInByte - 1);
    is_one_error_corrected = true;

    return decoded_block;
}

int HammingCoder::GetSizeOfEncodedBlock(int a) {
    return a + CeilDivide(GetAmountOfControlBits(a) + 1, kBitsInByte);
}

int HammingCoder::GetAmountOfControlBits(int a) {
    int length = a * kBitsInByte;
    int control_bits = CeiledLog2(length + 1);
    return CeiledLog2(length + control_bits + 1);
}


