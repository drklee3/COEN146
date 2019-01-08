# lab1

# Running

```bash
# compile source files
make

# generate random binary test files
head -c 10M < /dev/urandom > rand.bin

# run program
./main.o rand.bin rand.copy.bin

# verify checksums
sha256sum *.bin | tee SHA256SHUMS
```
