add_library(hamming_coder hamming_coder.cpp)
add_library(arg_parser arg_parser.cpp)
add_library(archiver archiver.cpp hamming_coder.cpp)
