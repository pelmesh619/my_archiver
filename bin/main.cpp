#include <iostream>

#include "../lib/arg_parser.h"
#include "../lib/archiver.h"


int CheckForExclusiveFlags(ArgumentParser::ArgParser& parser);

int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("=== Hyper Archiver ===");
    parser.AddHelp('h', "help", "Hyper archiver that use Hamming code - algorithm to correct one error in block and detect two errors in block");

    parser.AddStringArgument('f', "file", "Filename of archive");
    parser.AddFlag('l', "list", "Printing list of files in archive").Default(false);
    parser.AddFlag('c', "create", "Creation of new archive").Default(false);
    parser.AddFlag('x', "extract", "Extracting files from archive (if filename are not provided all files will be extracted)").Default(false);
    parser.AddFlag('a', "append", "Appending files into archive").Default(false);
    parser.AddFlag('d', "delete", "Removing files from archive").Default(false);
    parser.AddFlag('A', "concatenate", "Merging primary archive with inputted archives").Default(false);
    parser.AddIntArgument('q', "encode_size", "Size of block in bytes to encode (more size -> less size of all archive -> less error can be corrected)").Default(8);

    std::vector<std::string> filenames;

    parser.AddStringArgument("input", "Filenames of files/archives").MultiValue().Positional().StoreValues(filenames);

    bool result = parser.Parse(argc, argv);

    if (parser.Help()) {
        std::cout << parser.HelpDescription();
        return 0;
    }

    if (parser.error == ArgumentParser::ArgParserError::UnexpectedArgument) {
        std::cerr << "Unexpected argument is detected. Please read help message\n";
        std::cout << parser.HelpDescription();
        return 0;
    }

    int flag_amount = CheckForExclusiveFlags(parser);
    if (flag_amount == 0) {
        std::cerr << "You didn't include flag of action in arguments, i don't know what to do\n";
        std::cout << parser.HelpDescription();
        return 0;
    } else if (flag_amount != 1) {
        std::cerr << "You included too much flags of action in arguments, what should i do\n";
        std::cout << parser.HelpDescription();
        return 0;
    }

    Archiver hamarc(parser.GetIntValue("encode_size"), parser.GetStringValue("file"));

    if (parser.GetFlag("create")) {
        hamarc.Archive(filenames);
    } else if (parser.GetFlag("extract")) {
        hamarc.Dearchive(filenames);
    } else if (parser.GetFlag("concatenate")) {
        hamarc.Merge(filenames);
    } else if (parser.GetFlag("append")) {
        hamarc.AppendFiles(filenames);
    } else if (parser.GetFlag("delete")) {
        if (!filenames.empty()) {
            hamarc.DeleteFiles(filenames);
        } else {
            std::cout << "You are going to delete all files from archive: it is easier to do in Explorer" << std::endl;
        }
    } else if (parser.GetFlag("list")) {
        std::vector<FileHeader> files = hamarc.GetListOfFiles();

        std::string filename_string = "File name";
        int max_string_length = filename_string.size();
        for (FileHeader& f : files) {
            max_string_length = std::max(max_string_length, static_cast<int>(f.filename_size));
        }
        std::cout << "| " << filename_string << std::string(max_string_length - filename_string.size(), ' ')
                  << " |     Size in archive | Size of block       |" << std::endl;
        for (FileHeader& f : files) {
            std::cout << "| " << f.filename << std::string(max_string_length - f.filename_size, ' ') << " | "
                      << std::string(13 - HammingCoder::CeiledLog10(f.file_size), ' ') << f.file_size << " bytes | "
                      << std::string(13 - HammingCoder::CeiledLog10(f.block_size), ' ') << f.block_size << " bytes | "
                      << std::endl;
        }
    }

    return 0;
}

int CheckForExclusiveFlags(ArgumentParser::ArgParser& parser) {
    int counter = 0;

    std::vector<std::string> flags{"list", "create", "extract", "append", "delete", "concatenate"};

    for (const std::string& s : flags) {
        if (parser.GetFlag(s)) {
            counter++;
        }
    }

    return counter;

}
