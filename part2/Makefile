# Compilers
C_COMPILER = gcc
C_OPTIONS = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -m32 -c -MMD
ASM_COMPILER = nasm
ASM_OPTIONS = -f elf32

LINKER = ld
LINKER_OPTIONS = -m elf_i386 -T

# Source files
SRC_DIRECTORY = source/

C_FILES = main.c \
	screen_manager.c \
	print_manager.c \
	memory_manager.c \
	cursor.c \
	io.c \
	utils.c \
	interrupts.c \
	keyboard_interrupts.c \
	char_stacked_queue.c \
	keyboard_input.c \
	keyboard_map.c \
	tests.c \
	serial.c \
	gdt.c

ASM_FILES = boot.asm \
	interrupt_handlers.asm \
	gdt_flush.asm

LINKER_FILE = $(SRC_DIRECTORY)linker.ld

GRUB_CFG = $(SRC_DIRECTORY)grub.cfg

# Targets files
OBJ_DIRECTORY = obj/

KERNEL_NAME = kfs
KERNEL_VERSION = 2

ASM_OBJ = $(addprefix $(OBJ_DIRECTORY), $(ASM_FILES:.asm=.o))
C_OBJ = $(addprefix $(OBJ_DIRECTORY), $(C_FILES:.c=.o))
OBJ = $(ASM_OBJ) $(C_OBJ)
DEPS := ${OBJ:.o=.d}
QEMU := /usr/bin/qemu-system-x86_64

CODE_TARGET = $(OBJ_DIRECTORY)$(KERNEL_NAME)$(KERNEL_VERSION)

ISO_TARGET = $(KERNEL_NAME)$(KERNEL_VERSION).iso

# Rules
all: $(ISO_TARGET)

virtualisation: all
	unset GTK_PATH
	$(QEMU) -enable-kvm -cdrom $(ISO_TARGET) -m 1G -boot d -serial stdio -monitor pty

log: all
	unset GTK_PATH
	$(QEMU) -enable-kvm -cdrom $(ISO_TARGET) -m 1G -boot d -serial file:kernel.log -monitor stdio

$(ISO_TARGET): $(CODE_TARGET)
	mkdir -p $(OBJ_DIRECTORY)iso/boot/grub
	cp $(CODE_TARGET) $(OBJ_DIRECTORY)iso/boot
	cp $(GRUB_CFG) $(OBJ_DIRECTORY)iso/boot/grub/
	grub-mkrescue -o $(ISO_TARGET) $(OBJ_DIRECTORY)iso/

$(CODE_TARGET): $(C_OBJ) $(ASM_OBJ)
	$(LINKER) $(LINKER_OPTIONS) $(LINKER_FILE) -o $(CODE_TARGET) $(OBJ)

$(OBJ_DIRECTORY):
	mkdir -p $(OBJ_DIRECTORY)

$(ASM_OBJ): $(OBJ_DIRECTORY)%.o: $(SRC_DIRECTORY)%.asm | $(OBJ_DIRECTORY)
	$(ASM_COMPILER) $(ASM_OPTIONS) $< -o $@

$(C_OBJ): $(OBJ_DIRECTORY)%.o: $(SRC_DIRECTORY)%.c | $(OBJ_DIRECTORY)
	$(C_COMPILER) $(C_OPTIONS) $< -o $@

format:
	clang-format -i $(SRC_DIRECTORY)*.c $(SRC_DIRECTORY)*.h

clean:
	-rm $(DEPS)
	rm -rf $(OBJ_DIRECTORY)

fclean : clean
	rm -rf $(ISO_TARGET)

re: fclean all

.PHONY: all clean fclean re format log

-include $(DEPS)
