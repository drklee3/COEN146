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

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: [source] [destination]\n");
        return 1;
    }

    // read files
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

    // determine file size
    long file_size = getFileSize(source_fp);
    char* content = (char*)malloc(file_size + 1);

    // read content from source file
    fread(content, file_size, 1, source_fp);
    fclose(source_fp);

    // write content to dest file
    fwrite(content, file_size, 1, dest_fp);
    fclose(dest_fp);

    printf("Copied %s content to %s\n", argv[1], argv[2]);
}