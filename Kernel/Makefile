TARGET = x86_64-elf

OBJDIR   := build
SRCDIR   := src
ROOTSRC  := KernelMain.cpp

CXX = clang++
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu++17 -nostdlib -fno-exceptions -fno-rtti -I. -I./include/kernel -I./include/common

SRC_CPP := $(shell find $(SRCDIR) -name '*.cpp')

OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC_CPP:$(SRCDIR)/%=%))

all: kernel.elf

kernel.elf: $(OBJS) linker.ld
	ld.lld -T linker.ld --no-relax -nostdlib -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) kernel.elf

.PHONY: all clean