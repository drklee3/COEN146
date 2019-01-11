# lab1

## Instructions

This project consists of building an application to copy a binary file to another file.

Your code will take the following arguments: src file, dest file.

It will copy src to dest. It will use functions `fread` and `fwrite` and a char buffer of size 10 bytes.

## Running

```bash
# generate random binary 10 byte test file
head -c 10 < /dev/urandom > rand.bin

# compile source files
make

# run program
./main.o rand.bin rand.copy.bin

# verify checksums
sha256sum *.bin

# remove build and binary files
make clean
```
