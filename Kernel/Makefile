TARGET = x86_64-elf

CXX = clang++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu++17 -nostdlib -fno-exceptions -fno-rtti -I.

LD = ld.lld
LDFLAGS = -T linker.ld --no-relax -nostdlib

all: kernel.elf

kernel.elf: kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

kernel.o: KernelMain.cpp BootInfo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f ./build/kernel.o ./build/kernel.elf