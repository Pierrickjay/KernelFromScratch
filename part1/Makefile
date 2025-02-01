
C_COMPILER = gcc
C_OPTION = -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -m32 -c 
ASM_COMPILER= nasm
ASM_OPTION= -f elf32

SRC_DIRECTORY=source/
OBJ_DIRECTORY=obj/

LINKER=ld
LINKER_OPTION=-m elf_i386 -T

C_FILE=

LINKER_FILE=$(SRC_DIRECTORY)linker.ld

TARGET=kfs1

all: $(TARGET)

# TRouvre une autre maniere que l'etoile
$(TARGET):
	mkdir -p $(OBJ_DIRECTORY)iso/boot/grub
	$(ASM_COMPILER) $(ASM_OPTION) $(SRC_DIRECTORY)boot.asm -o $(OBJ_DIRECTORY)boot.o
	$(C_COMPILER) $(C_OPTION) $(SRC_DIRECTORY)main.c -o $(OBJ_DIRECTORY)main.o
	$(C_COMPILER) $(C_OPTION) $(SRC_DIRECTORY)screen_manager.c -o $(OBJ_DIRECTORY)screen_manager.o
	$(LINKER) $(LINKER_OPTION) $(LINKER_FILE) -o $(TARGET) obj/*.o
	cp kfs1 $(OBJ_DIRECTORY)iso/boot
	cp $(SRC_DIRECTORY)grub.cfg $(OBJ_DIRECTORY)iso/boot/grub/
	grub-mkrescue -o kfs1.iso $(OBJ_DIRECTORY)iso/

clean:
	rm -rf $(OBJ_DIRECTORY)

fclean : clean
	rm -rf $(TARGET)
	rm -rf $(TARGET).iso

re: fclean all

# -fno-builtin
# • -fno-exception
# • -fno-stack-protector
# • -fno-rtti
# • -nostdlib
# • -nodefaultlibs

# trouver leur equivalent rust
