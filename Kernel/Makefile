TARGET   := x86_64-elf               
OBJDIR   := build
SRCDIR   := src
ROOTSRC  := KernelMain.cpp          

CXX      := clang++
CC       := clang                  
LD       := ld.lld

COMMON   := -target $(TARGET) -ffreestanding -nostdlib \
            -m64 -mcmodel=kernel -mno-red-zone          

CXXFLAGS := $(COMMON) -O2 -Wall -Wextra -std=gnu++20 \
            -fno-exceptions -fno-rtti \
            -I. -I./include/kernel -I./include/common

ASFLAGS  := $(COMMON) -x assembler-with-cpp

SRC_CPP  := $(shell find $(SRCDIR) -name '*.cpp')
SRC_S    := $(shell find $(SRCDIR) -name '*.S')

OBJS_CPP := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC_CPP))
OBJS_S   := $(patsubst $(SRCDIR)/%.S,   $(OBJDIR)/%.o, $(SRC_S))

OBJS     := $(OBJS_CPP) $(OBJS_S)


all: kernel.elf

kernel.elf: $(OBJS) linker.ld
	$(LD) -T linker.ld --no-relax -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	@mkdir -p $(dir $@)
	$(CC)  $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) kernel.elf

.PHONY: all clean
