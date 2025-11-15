.NOTPARALLEL:

# Compilers
C_COMPILER = gcc
C_OPTIONS = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -m32 -c -MMD \
	-I$(SRC_DIRECTORY) \
	-I$(SRC_DIRECTORY)boot \
	-I$(SRC_DIRECTORY)cpu \
	-I$(SRC_DIRECTORY)drivers/keyboard \
	-I$(SRC_DIRECTORY)drivers/serial \
	-I$(SRC_DIRECTORY)drivers/timer \
	-I$(SRC_DIRECTORY)io \
	-I$(SRC_DIRECTORY)display \
	-I$(SRC_DIRECTORY)shell \
	-I$(SRC_DIRECTORY)lib \
	-I$(SRC_DIRECTORY)tests
ASM_COMPILER = nasm
ASM_OPTIONS = -f elf32

LINKER = ld
LINKER_OPTIONS = -m elf_i386 -T

# Source files
SRC_DIRECTORY = source/

C_FILES = main.c \
	display/screen_manager.c \
	display/print_manager.c \
	lib/memory_manager.c \
	display/cursor.c \
	io/io.c \
	lib/utils.c \
	cpu/interrupts.c \
	cpu/signal_handler.c \
	cpu/signal_scheduler.c \
	cpu/signal_handlers.c \
	cpu/exception_handlers.c \
	cpu/cpu_exception_handlers.c \
	cpu/panic.c \
	drivers/keyboard/keyboard_interrupts.c \
	drivers/timer/timer.c \
	lib/char_stacked_queue.c \
	drivers/keyboard/keyboard_input.c \
	drivers/keyboard/keyboard_text_mode_map.c \
	drivers/keyboard/keyboard_map.c \
	tests/tests.c \
	drivers/serial/serial.c \
	cpu/gdt.c \
	shell/mini_minishell.c \
	lib/string.c

ASM_FILES = boot/boot.asm \
	cpu/interrupt_handlers.asm \
	cpu/exception_stubs.asm \
	cpu/gdt_flush.asm

LINKER_FILE = $(SRC_DIRECTORY)boot/linker.ld

GRUB_CFG = $(SRC_DIRECTORY)boot/grub.cfg

# Targets files
OBJ_DIRECTORY = obj/

KERNEL_NAME = kfs
KERNEL_VERSION = 4

ASM_OBJ = $(addprefix $(OBJ_DIRECTORY), $(ASM_FILES:.asm=.o))
C_OBJ = $(addprefix $(OBJ_DIRECTORY), $(C_FILES:.c=.o))
OBJ = $(ASM_OBJ) $(C_OBJ)
DEPS := ${OBJ:.o=.d}
QEMU := /usr/bin/qemu-system-x86_64

CODE_TARGET = $(OBJ_DIRECTORY)$(KERNEL_NAME)$(KERNEL_VERSION)

ISO_TARGET = $(KERNEL_NAME)$(KERNEL_VERSION).iso

# Rules
all: compile_commands.json $(ISO_TARGET)

# Regenerate compile_commands.json when Makefile or source files change
compile_commands.json: Makefile $(addprefix $(SRC_DIRECTORY), $(C_FILES))
	@echo "Regenerating compile_commands.json..."
	@bear -- $(MAKE) --no-print-directory re > /dev/null 2>&1 || true

virtualisation: all
	$(QEMU) -enable-kvm -cdrom $(ISO_TARGET) -m 1G -boot d -serial stdio -monitor pty

log: all
	$(QEMU) -enable-kvm -cdrom $(ISO_TARGET) -m 1G -boot d -serial file:kernel.log -monitor stdio

$(ISO_TARGET): $(CODE_TARGET)
	mkdir -p $(OBJ_DIRECTORY)iso/boot/grub
	cp $(CODE_TARGET) $(OBJ_DIRECTORY)iso/boot
	cp $(GRUB_CFG) $(OBJ_DIRECTORY)iso/boot/grub/
	grub-mkrescue --compress=xz --fonts= --themes= --locales= -o $(ISO_TARGET) $(OBJ_DIRECTORY)iso/

$(CODE_TARGET): $(C_OBJ) $(ASM_OBJ)
	$(LINKER) $(LINKER_OPTIONS) $(LINKER_FILE) -o $(CODE_TARGET) $(OBJ)

$(OBJ_DIRECTORY):
	mkdir -p $(OBJ_DIRECTORY) \
		$(OBJ_DIRECTORY)boot \
		$(OBJ_DIRECTORY)cpu \
		$(OBJ_DIRECTORY)drivers/keyboard \
		$(OBJ_DIRECTORY)drivers/serial \
		$(OBJ_DIRECTORY)drivers/timer \
		$(OBJ_DIRECTORY)io \
		$(OBJ_DIRECTORY)display \
		$(OBJ_DIRECTORY)shell \
		$(OBJ_DIRECTORY)lib \
		$(OBJ_DIRECTORY)tests

$(ASM_OBJ): $(OBJ_DIRECTORY)%.o: $(SRC_DIRECTORY)%.asm | $(OBJ_DIRECTORY)
	$(ASM_COMPILER) $(ASM_OPTIONS) $< -o $@

$(C_OBJ): $(OBJ_DIRECTORY)%.o: $(SRC_DIRECTORY)%.c | $(OBJ_DIRECTORY)
	$(C_COMPILER) $(C_OPTIONS) $< -o $@

format:
	find $(SRC_DIRECTORY) -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	-rm $(DEPS)
	rm -rf $(OBJ_DIRECTORY)

fclean : clean
	rm -rf $(ISO_TARGET) compile_commands.json

re: fclean all

.PHONY: all clean fclean re format log

-include $(DEPS)
