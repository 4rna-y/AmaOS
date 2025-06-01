cd ./Kernel

make clean
make

mv kernel.o ./build/kernel.o
mv kernel.elf ./build/kernel.elf