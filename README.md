This archiver was written as one of laboratory work in the first semester in ITMO University

It uses redundant error-correcting Hamming code and can archive a lot of files creating a single `.haf` file

Use CMake to built .exe file

### Usage

**-f, --file=[ARHCNAME]**  - filename of archive

**-c, --create**           - creating a new archive

**-l, --list**             - list all files in an existing archive

**-x, --extract**          - extract file from an existing archive (make dearchving)

**-a, --append**           - append file in an archive file

**-d, --delete**           - delete file from an archive (without extracting)

**-A, --concatenate**      - merge 2 or move archives

**-input**                 - input files to archive/extract or archive files to merge (argument can be positional)

**-q, encode_size**        - size of block to encode in Hamming code (less size of block -> more bytes per block will be added -> more volume -> more error in archive can be corrected)

### Examples of execution

*hamarc --create --file=ARCHIVE FILE1 FILE2 FILE3*

*hamarc -l -f ARCHIVE*

*hamarc --concantenate ARCHIVE1 ARCHIVE2 -f ARCHIVE3*