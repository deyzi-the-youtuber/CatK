MKDIR = mkdir -p
CP = cp -f
RM = rm
RM_FORCE = rm -rf

# assembler
ASM = nasm

# compiler

# !!! DO NOT CHANGE THIS !!!
# i don't care if you can't use clang, i don't care if you hate clang.
# this project targets clang and clang only. you will not be helped if it fails.
# gcc broke this project. i am not fixing it again. - irix
CC = clang

# linker. this can be GNU ld or LLVM lld.
# lld is now preferred as it works everywhere
LD = ld

# sources
SRC = src
ASM_SRC = $(SRC)/asm

# objects
OBJ = obj
ASM_OBJ = $(OBJ)/asm

CONFIG = ./config

OUT = out

INC = ./include

INCLUDE =-I$(INC)

# assembler flags
ASM_FLAGS = -f elf32

QEMU_FLAGS = -cdrom $(OUT)/catkernel.iso -serial stdio

# compiler flags
CC_FLAGS = $(INCLUDE) --target=i686-pc-none-elf -march=i686 -std=gnu99 -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -Werror=implicit-function-declaration -ferror-limit=9999

ifeq ($(DEBUG), 1)
CC_FLAGS += -g
endif

ifeq ($(OS), Windows_NT)
override SKIP_ISO = 1
override SKIP_MB_CHECK = 1
endif

# linker flags, for linker add linker.ld file too
LD_FLAGS = -m elf_i386 -T $(CONFIG)/linker.ld -nostdlib

# target file to create in linking
TARGET = $(OUT)/catkernel.bin

# iso file target to create
TARGET_ISO = $(OUT)/catkernel.iso
ISO_DIR = $(OUT)/isodir

$(shell $(MKDIR) $(OBJ) $(OUT))

# automatically find all C source files in src/ and its subdirectories

# deyzi-the-youtuber: im not used to the wildcard and patsubst functions so this might be a bit wierd
C_SOURCES := $(wildcard $(SRC)/**/*.c $(SRC)/*.c $(SRC)/proc/*.c $(SRC)/meta/*.c $(SRC)/mm/*.c $(SRC)/lib/*.c $(SRC)/fs/*.c $(SRC)/drivers/*.c $(SRC)/drivers/**/*.c $(SRC)/arch/x86/*.c $(SRC)/programs/**/*.c $(SRC)/programs/*.c)
ASM_SOURCES := $(wildcard $(ASM_SRC)/**/*.asm $(ASM_SRC)/*.asm)

# generate object file names from source file names
OBJECTS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst $(ASM_SRC)/%.asm, $(ASM_OBJ)/%.o, $(ASM_SOURCES))


all: $(TARGET_ISO)

$(TARGET_ISO): $(TARGET)
	@printf "[ building ISO... ]\n"
ifneq ($(SKIP_ISO), 1)
	$(MKDIR) $(ISO_DIR)/boot/grub
	$(CP) $(TARGET) $(ISO_DIR)/boot/
	$(CP) $(CONFIG)/grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(TARGET_ISO) $(ISO_DIR)
	$(RM) $(TARGET)
else
	@printf "[ iso build skipped ]\n"
endif

$(TARGET): $(ASM_OBJECTS) $(OBJECTS)
	@printf "[ linking... ]\n"
	$(LD) $(LD_FLAGS) -o $(TARGET) $^
	@printf "[ checking multiboot signature... ]\n"
ifneq ($(SKIP_MB_CHECK), 1)
	grub-file --is-x86-multiboot $(TARGET)
else
	@printf "[ multiboot check skipped ]\n"
endif

$(ASM_OBJ)/%.o: $(ASM_SRC)/%.asm
	@printf "[ $< ]\n"
	$(MKDIR) $(dir $@)
	$(ASM) $(ASM_FLAGS) $< -o $@
	@printf "\n"

$(OBJ)/%.o: $(SRC)/%.c
	@printf "[ $< ]\n"
	$(MKDIR) $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@
	@printf "\n"

qemu:
	@qemu-system-i386 $(QEMU_FLAGS)

clean:
	$(RM_FORCE) $(OBJ)
	$(RM_FORCE) $(OUT)