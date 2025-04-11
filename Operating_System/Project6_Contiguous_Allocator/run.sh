echo "gcc -o allocator allocator.c"
echo "./allocator 1000"
echo "You can also use your arguments by './allocator <size of total memory>'"
echo ""
echo ""

gcc -o allocator allocator.c
./allocator 1000
