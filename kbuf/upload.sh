#익스코드 컴파일 후 fs에 넣고 cpio로 패킹
# Check if ex.c exists in the current directory
if [ -f "test.c" ]; then
    # Compile ex.c to create an executable named ex
    musl-gcc ./test.c -o ./ex -static
    # Copy the compiled ex binary to the fs directory
    cp ./ex ./fs/
    echo "Compilation and copying completed."
else
    echo "ex.c does not exist in the current directory. Skipping compilation and copying."
fi
cd fs
find . -print0 | cpio -o --format=newc --null  > ../rootfs_updated.cpio
cd ../
