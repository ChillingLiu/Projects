echo "gcc -o manager manager.c"
echo "./manager addresses.txt"
echo "You can also use your input file by './manager <input file>'"
echo ""
echo ""

gcc -o manager manager.c
./manager addresses.txt
