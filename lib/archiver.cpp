#include "archiver.h"


bool contains(const std::vector<std::string>& vec, const std::string& s) {
    for (const std::string& i : vec) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

std::fstream& operator<<(std::fstream& stream, const ArchiveHeader& header) {
    Archiver::WriteAsBinary(stream, header.archive_type);
    Archiver::WriteAsBinary(stream, header.header_size);
    Archiver::WriteAsBinary(stream, header.file_amount);

    return stream;
}

std::fstream& operator<<(std::fstream& stream, const FileHeader& header) {
    Archiver::WriteAsBinary(stream, header.header_size);
    Archiver::WriteAsBinary(stream, header.file_size);
    Archiver::WriteAsBinary(stream, header.filename_size);
    Archiver::WriteAsBinary(stream, header.block_size);
    stream << header.filename;

    return stream;
}

Archiver::Archiver(int block_size, const std::string& archive_filename) {
    this->block_size = block_size;
    this->archive_filename = archive_filename;
}

void Archiver::Archive(const std::vector<std::string>& filenames) {
    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out);
    if (!archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return;
    }

    ArchiveHeader header;
    file_amount = filenames.size();
    (*archive_file) << header;

    for (const std::string& filename : filenames) {
        ArchiveFile(filename);
    }
    ChangeFileAmount(file_amount);
    std::cout << file_amount << " files were archived into '" << archive_filename << "'" << std::endl;
}

void Archiver::ArchiveFile(const std::string& filename) {
    std::fstream file(filename.c_str(), std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        std::cerr << "File '" << filename << "' to dearchive file cannot be opened" << std::endl;
        file_amount--;
        return;
    }

    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::end);
    file_size = file.tellg() - file_size;

    int encoded_file_size = HammingCoder::GetSizeOfEncodedBlock(block_size) * (file_size / block_size) +
            (file_size % block_size > 0 ? HammingCoder::GetSizeOfEncodedBlock(file_size % block_size) : 0);

    FileHeader header;
    header.header_size = sizeof(header.header_size) + sizeof(header.file_size) +
            sizeof(header.filename_size) + sizeof(header.block_size) + filename.size();
    header.block_size = block_size;
    header.file_size = encoded_file_size;
    header.filename_size = filename.size();
    header.filename = filename;

    (*archive_file) << header;

    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> block;
    std::vector<uint8_t> encoded_block;
    uint8_t byte;

    HammingCoder coder;

    long long counter = 0;
    long long next_percent_milestone = 0;

    while (!file.eof()) {
        if (counter >= file_size * next_percent_milestone / 100) {
            std::cout << "Archiving '" << filename << "':\t" << next_percent_milestone << "%\t[";
            for (int i = 0; i < 100; i += 20)
                std::cout << (i < next_percent_milestone ? "#" : "=");
            std::cout << "]\t" << counter << " / " << file_size << std::endl;

            next_percent_milestone += 20;
        }

        for (int i = 0; i < block_size; i++) {
            if (file.eof()) {
                break;
            }
            byte = file.get();
            block.push_back(byte);
            if (file.peek() == -1) {
                break;
            }
        }

        encoded_block = coder.EncodeBlock(block);

        for (uint8_t i : encoded_block) {
            (*archive_file) << i;
        }

        counter += block.size();
        block.clear();

    }
    std::cout << "'" << filename << "' was successfully archived!" << std::endl;
}

void Archiver::DearchiveFile(const std::vector<std::string>& filenames) {
    uint32_t header_size;
    uint32_t file_size;
    uint32_t filename_size;
    uint32_t block_size;

    ReadAsBinary(ReadAsBinary((*archive_file), header_size), file_size);
    ReadAsBinary(ReadAsBinary((*archive_file), filename_size), block_size);
    std::string filename;

    for (uint32_t i = 0; i < filename_size; i++) {
        char a;
        (*archive_file) >> a;
        if (archive_file->eof()) {
            return;
        }
        filename += a;
    }

    if (!filenames.empty() && !contains(filenames, filename)) {
        std::streampos current_file_pos = archive_file->tellg();
        current_file_pos += file_size - 1;
        archive_file->seekg(current_file_pos);
        return;
    }
    int encoded_block_size = HammingCoder::GetSizeOfEncodedBlock(block_size);

    std::string dearchived_filename = kDearchivedFilePrefix + filename;
    std::fstream file(dearchived_filename, std::ios::binary | std::ios::out);

    if (!file.is_open()) {
        std::cerr << "File '" << dearchived_filename << "' to dearchive file cannot be opened" << std::endl;
        std::streampos current_file_pos = archive_file->tellg();
        current_file_pos += filename_size - 1;
        archive_file->seekg(current_file_pos);
        return;
    }

    std::vector<uint8_t> block;
    std::vector<uint8_t> encoded_block;
    uint8_t byte;

    HammingCoder coder;

    long long counter = 0;
    long long next_percent_milestone = 0;
    for (int j = 0; j < file_size / encoded_block_size; j++) {
        if (counter >= file_size * next_percent_milestone / 100) {
            std::cout << "Dearchiving '" << filename << "':\t" << next_percent_milestone << "%\t[";
            for (int i = 0; i < 100; i += 20)
                std::cout << (i < next_percent_milestone ? "#" : "=");
            std::cout << "]\t" << counter << " / " << file_size << std::endl;

            next_percent_milestone += 20;
        }
        for (int i = 0; i < encoded_block_size; i++) {
            byte = archive_file->get();
            encoded_block.push_back(byte);
            if (archive_file->peek() == -1) {
                break;
            }
        }

        block = coder.DecodeBlock(encoded_block);
        if (coder.are_there_two_errors) {
            std::cerr << "In file '" << filename << "' two errors detected, dearchiving continues" << std::endl;
        }
        if (coder.is_one_error_corrected) {
            std::cerr << "In file '" << filename << "' one error corrected, dearchiving continues" << std::endl;
        }

        for (uint8_t i : block) {
            file << i;
        }

        counter += encoded_block.size();
        encoded_block.clear();
        if (archive_file->peek() == -1) {
            break;
        }

    }

    for (int i = 0; i < std::min(static_cast<uint32_t>(encoded_block_size), file_size % encoded_block_size); i++) {
        byte = archive_file->get();
        encoded_block.push_back(byte);
        if (archive_file->peek() == -1) {
            break;
        }
    }

    if (!encoded_block.empty()) {
        block = coder.DecodeBlock(encoded_block);
        if (coder.are_there_two_errors) {
            std::cerr << "In file '" << filename << "' two errors detected, dearchiving continues" << std::endl;
        }
        if (coder.is_one_error_corrected) {
            std::cerr << "In file '" << filename << "' one error corrected, dearchiving continues" << std::endl;
        }

        for (uint8_t i : block) {
            file << i;
        }
    }

    dearchived_file_amount++;
    std::cout << "'" << filename << "' was successfully dearchived!" << std::endl;
}

Archiver::~Archiver() {
    delete archive_file;
}

void Archiver::Dearchive(const std::vector<std::string>& filenames) {
    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::in);
    if (!archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return;
    }

    ArchiveHeader header;
    uint32_t a;
    ReadAsBinary((*archive_file), a);
    if (a != header.archive_type) {
        std::cerr << "It's seems to be that this 'archive' file ('" + archive_filename + "') is not archive .haf" << std::endl;
        return;
    }

    ReadAsBinary((*archive_file), a);

    ReadAsBinary((*archive_file), this->file_amount);

    while (!archive_file->eof()) {
        DearchiveFile(filenames);
        if (!filenames.empty() && filenames.size() == dearchived_file_amount) {
            break;
        }
    }

    std::cout << "Dearchived " << dearchived_file_amount << " files in total from '" << archive_filename << "'" << std::endl;

}

std::fstream& Archiver::WriteAsBinary(std::fstream& stream, uint32_t value) {
    for (int i = sizeof(value) - 1; i >= 0; i--) {
        stream << static_cast<uint8_t>(value >> (i * kBitsInByte));
    }

    return stream;
}
std::fstream& Archiver::WriteAsBinary(std::fstream& stream, uint16_t value) {
    for (int i = sizeof(value) - 1; i >= 0; i--) {
        stream << static_cast<uint8_t>(value >> (i * kBitsInByte));
    }

    return stream;
}

std::fstream& Archiver::ReadAsBinary(std::fstream& stream, uint32_t& value) {
    uint8_t a;
    value = 0;
    for (int i = sizeof(value) - 1; i >= 0; i--) {
        value <<= kBitsInByte;
        a = stream.get();
        value |= a;
        if (stream.peek() == -1) {
            break;
        }
    }

    return stream;
}

std::fstream& Archiver::ReadAsBinary(std::fstream& stream, uint16_t& value) {
    uint8_t a;
    value = 0;
    for (int i = sizeof(value) - 1; i >= 0; i--) {
        value <<= kBitsInByte;
        a = stream.get();
        value |= a;
        if (stream.peek() == -1) {
            break;
        }
    }

    return stream;
}

void Archiver::AppendFiles(const std::vector<std::string>& filenames) {
    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out | std::ios::in);
    if (archive_file == nullptr || !archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return;
    }

    ArchiveHeader header;
    header.file_amount = filenames.size();

    uint32_t a;
    ReadAsBinary((*archive_file), a);
    if (a != header.archive_type) {
        std::cerr << "It's seems to be that this 'archive' file ('" + archive_filename + "') is not archive .haf" << std::endl;
        return;
    }
    ReadAsBinary((*archive_file), a);
    uint32_t old_file_amount;
    ReadAsBinary((*archive_file), old_file_amount);

    this->file_amount = old_file_amount;
    this->file_amount += filenames.size();

    archive_file->seekp(0, std::ios::end);
    for (const std::string& filename : filenames) {
        ArchiveFile(filename);
    }
    ChangeFileAmount(this->file_amount);
    std::cout << this->file_amount - old_file_amount << " files were appended into '" << archive_filename
              << "', in total - " << this->file_amount << " files " << std::endl;
}

void Archiver::ChangeFileAmount(uint32_t new_file_amount) {
    if (archive_file == nullptr) {
        return;
    }
    long long current_pos = archive_file->tellg();
    archive_file->seekg(8);
    WriteAsBinary(*archive_file, new_file_amount);
    archive_file->seekg(current_pos);
}

std::vector<FileHeader> Archiver::GetListOfFiles() {
    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out | std::ios::in);
    if (archive_file == nullptr || !archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return {};
    }
    std::vector<FileHeader> files;

    ArchiveHeader header;
    uint32_t a;
    ReadAsBinary((*archive_file), a);
    if (a != header.archive_type) {
        std::cerr << "It's seems to be that this 'archive' file ('" + archive_filename + "') is not archive .haf" << std::endl;
        return {};
    }
    ReadAsBinary((*archive_file), a);
    ReadAsBinary(*archive_file, this->file_amount);


    while (!archive_file->eof()) {
        FileHeader file_header = GetFileHeader();
        if (file_header.filename.empty()) {
            break;
        }
        files.push_back(file_header);
    }
    std::cout << this->file_amount << " files are detected in '" << archive_filename << "'" << std::endl;

    return files;
}

FileHeader Archiver::GetFileHeader() {
    FileHeader header;
    if (!archive_file) {
        return header;
    }

    ReadAsBinary(ReadAsBinary((*archive_file), header.header_size), header.file_size);
    ReadAsBinary(ReadAsBinary((*archive_file), header.filename_size), header.block_size);

    for (uint32_t i = 0; i < header.filename_size; i++) {
        if (archive_file->eof()) {
            break;
        }
        char a;
        (*archive_file) >> a;
        header.filename += a;
    }

    if (!archive_file->eof()) {
        std::streampos current_file_pos = archive_file->tellg();
        current_file_pos += header.file_size;
        archive_file->seekg(current_file_pos);
    }

    return header;
}


void Archiver::Merge(const std::vector<std::string>& archive_filenames) {
    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out);
    if (!archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' (destination of merging) cannot be opened" << std::endl;
        return;
    }

    ArchiveHeader header;
    header.file_amount = 0;
    file_amount = 0;
    (*archive_file) << header;

    for (const std::string& filename : archive_filenames) {
        MergeArchive(filename);
    }
    ChangeFileAmount(file_amount);
    std::cout << file_amount << " files from " << archive_filenames.size()
              << " archives were archived into '" << archive_filename << "'" << std::endl;
}

void Archiver::MergeArchive(const std::string& filename) {
    std::fstream archive(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!archive.is_open()) {
        std::cerr << "Archive file '" << filename << "' (source of merging) cannot be opened" << std::endl;
        return;
    }

    ArchiveHeader header;
    uint32_t a;
    ReadAsBinary(archive, a);
    if (a != header.archive_type) {
        std::cerr << "It's seems to be that this 'archive' file ('" + archive_filename + "') is not archive .haf" << std::endl;
        return;
    }

    ReadAsBinary(archive, a);
    ReadAsBinary(archive, a);
    this->file_amount += a;

    while (!archive.eof()) {
        (*archive_file) << static_cast<uint8_t>(archive.get());
        if (archive.peek() == -1) {
            break;
        }
    }
}

void Archiver::DeleteFiles(const std::vector<std::string> &filenames) {
    std::string temp_archive_filename = "[temp] " + archive_filename;

    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out | std::ios::in);
    if (archive_file == nullptr || !archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return;
    }
    std::fstream temp_archive_file(temp_archive_filename, std::ios::binary | std::ios::out);
    if (!temp_archive_file.is_open()) {
        std::cerr << "Temporary file '" << temp_archive_filename << "' cannot be opened, I can not delete files from archive" << std::endl;
        return;
    }

    ArchiveHeader header;
    uint32_t a;
    ReadAsBinary((*archive_file), a);
    if (a != header.archive_type) {
        std::cerr << "It's seems to be that this 'archive' file ('" + archive_filename + "') is not archive .haf" << std::endl;
        return;
    }
    ReadAsBinary((*archive_file), a);
    ReadAsBinary(*archive_file, this->file_amount);

    temp_archive_file << header;

    while (!archive_file->eof()) {
        DeleteFile(filenames, temp_archive_file);
    }
    temp_archive_file.close();
    temp_archive_file = std::fstream(temp_archive_filename, std::ios::binary | std::ios::in);
    temp_archive_file.seekg(0);
    delete archive_file;

    archive_file = new std::fstream(archive_filename, std::ios::binary | std::ios::out);
    if (archive_file == nullptr || !archive_file->is_open()) {
        std::cerr << "Archive file '" << archive_filename << "' cannot be opened" << std::endl;
        return;
    }

    while (!temp_archive_file.eof()) {
        (*archive_file) << static_cast<uint8_t>(temp_archive_file.get());
        if (temp_archive_file.peek() == -1) {
            break;
        }
    }

    ChangeFileAmount(file_amount);
    std::cout << "Provided files were deleted from '" << archive_filename << "', in total - " << this->file_amount << " files " << std::endl;
}

void Archiver::DeleteFile(const std::vector<std::string>& filenames, std::fstream& temp_file) {
    FileHeader header;

    ReadAsBinary(ReadAsBinary((*archive_file), header.header_size), header.file_size);
    ReadAsBinary(ReadAsBinary((*archive_file), header.filename_size), header.block_size);

    for (uint32_t i = 0; i < header.filename_size; i++) {
        char a;
        (*archive_file) >> a;
        if (archive_file->eof()) {
            return;
        }
        header.filename += a;
    }

    if (contains(filenames, header.filename)) {
        std::streampos current_file_pos = archive_file->tellg();
        current_file_pos += header.file_size;
        archive_file->seekg(current_file_pos);
        std::cout << "'" << header.filename << "' was successfully deleted!" << std::endl;
        file_amount--;
        return;
    }
    temp_file << header;
    for (int i = 0; i < header.file_size; i++) {
        temp_file << static_cast<uint8_t>(archive_file->get());
        if (archive_file->peek() == -1) {
            break;
        }
    }
}


