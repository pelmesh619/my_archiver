#pragma once

#include <vector>
#include <iostream>
#include <cinttypes>
#include <fstream>

#include "hamming_coder.h"

const std::string kDearchivedFilePrefix = "[dearchived] ";

bool contains(const std::vector<std::string>& vec, const std::string& s);

struct ArchiveHeader {
    const uint32_t archive_type = 0x48414631;
    const uint32_t header_size = 12;
    uint32_t file_amount;
};

std::fstream& operator<<(std::fstream& stream, const ArchiveHeader& header);

struct FileHeader {
    uint32_t header_size;
    uint32_t file_size;
    uint32_t filename_size;
    uint32_t block_size;
    std::string filename;
};

std::fstream& operator<<(std::fstream& stream, const FileHeader& header);


class Archiver {
public:
    uint32_t file_amount = 0;
    uint32_t dearchived_file_amount = 0;

    static std::fstream& WriteAsBinary(std::fstream& stream, uint32_t value);
    static std::fstream& WriteAsBinary(std::fstream& stream, uint16_t value);
    static std::fstream& ReadAsBinary(std::fstream& stream, uint32_t& value);
    static std::fstream& ReadAsBinary(std::fstream& stream, uint16_t& value);

    Archiver(int block_size, const std::string& archive_filename);
    ~Archiver();
    void Archive(const std::vector<std::string>& filenames);
    void Dearchive(const std::vector<std::string>& filenames);
    void Merge(const std::vector<std::string>& archive_filenames);
    void AppendFiles(const std::vector<std::string>& filenames);
    void DeleteFiles(const std::vector<std::string>& filenames);
    std::vector<FileHeader> GetListOfFiles();

private:
    int block_size = 8;
    std::string archive_filename;
    std::fstream* archive_file = nullptr;
    int pointer = 0;

    void ChangeFileAmount(uint32_t new_file_amount);

    void ArchiveFile(const std::string& filename);
    void DearchiveFile(const std::vector<std::string>& filenames);
    FileHeader GetFileHeader();
    void MergeArchive(const std::string& filename);
    void DeleteFile(const std::vector<std::string>& filenames, std::fstream& temp_file);

};
