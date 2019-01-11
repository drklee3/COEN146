/**
 * Derrick Lee, F 2:15PM
 */

#include <stdio.h>
#include <stdlib.h>

/** 
 * Gets the file size from a file pointer
 * @param FILE* f a file pointer
 * @return Size of a file
 */
long getFileSize(FILE* f) {
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    return fsize;
}

/**
 * Gets the number of bytes to copy
 * @param long written number of bytes already copied
 * @param long size file size by number of bytes
 * @return number of bytes
 */
long getBytesToCopy(long written, long size) {
    long bytes = 10; // default size of 10
    long diff = size - written;

    // check if < bytes remaining as to not copy over extra
    if (diff < 10) {
        bytes = diff;
    }

    return bytes;
}

/**
 * Checks if there are errors reading or writing
 * @param size_t read number of objects read successfully
 * @param size_t write number of objects written successfully
 * @return true if there was an error
 */
bool hasErr(size_t read, size_t write) {
    if (read != 1) {
        printf("Failed reading\n");
        return true;
    }

    if (write != 1) {
        printf("Failed writing\n");
        return true;
    }

    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: [source] [destination]\n");
        return 1;
    }

    // open files
    FILE* source_fp = fopen(argv[1], "rb");

    if (!source_fp) {
        printf("Failed to open source file\n");
        return 1;
    }

    FILE* dest_fp = fopen(argv[2], "wb");

    if (!dest_fp) {
        printf("Failed to open destination file\n");
        return 1;
    }

    // determine file size and allocate memory
    long file_size = getFileSize(source_fp);
    // dynamic buffer size, can be used to copy all at once
    // char* content = (char*)malloc(file_size + 1);

    // fixed buffer size
    char* content[10];
    long bytes_written = 0;

    long bytes;
    size_t fread_bytes, fwrite_bytes;

    // copy bytes by every max 10 bytes
    while (bytes_written < file_size) {
        // get number of bytes to copy, equivalent to max(10, remaining_bytes)
        bytes = getBytesToCopy(bytes_written, file_size);

        // read/write, can also use fread_bytes as the # of bytes to write
        // but this is kinda "safer" maybe
        fread_bytes  =  fread(content, bytes, 1, source_fp);
        fwrite_bytes = fwrite(content, bytes, 1, dest_fp);

        // check if there are errors reading / writing
        if (hasErr(fread_bytes, fwrite_bytes)) {
            return 1;
        }

        bytes_written += bytes;
    }

    // close files
    fclose(source_fp);
    fclose(dest_fp);

    printf("Copied %s content to %s\n", argv[1], argv[2]);
}