GPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -fleading-underscore -fno-exceptions -fno-builtin -nostdlib -fno-rtti -fno-pie

ASPARAMS = --32
LDPARAMS = -melf_i386  -no-pie

objects = obj/loader.o \
 		  obj/kernel.o \
		  obj/gdt.o \
		  obj/memorymanager.o \
		  obj/syscalls.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/pci.o \
		  obj/hardwarecommunication/multitasking.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/mmioutils.o \
		  obj/hardwarecommunication/times.o \
		  obj/hardwarecommunication/sound.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/vga.o \
		  obj/drivers/svga.o \
		  obj/drivers/driver.o \
		  obj/drivers/ata.o \
		  obj/drivers/serialport.o \
		  obj/drivers/network/amd_am79c973.o \
		  obj/drivers/network/intel_e1000.o \
		  obj/drivers/network/realtek_8139.o \
		  obj/gui/widget.o \
		  obj/gui/stringtext.o \
		  obj/gui/window.o \
		  obj/gui/desktop.o \
		  obj/gui/button.o \
		  obj/gui/list.o \
		  obj/gui/scrollbar.o \
		  obj/filesystem/dospart.o\
		  obj/filesystem/fat.o \

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ ${GPPARAMS} -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as ${ASPARAMS} -o $@ -c $<

kernel.bin: linker.ld ${objects}
	ld ${LDPARAMS} -T $< -o $@ font.o ${objects}

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "my os" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: kernel.iso
	(killall qemu-system-i386 && sleep 1) || true
	qemu-system-i386 -cdrom $< -boot d -m 512 -nic model=pcnet &

.PHONY: clean
clean:
	(killall qemu-system-i386 && sleep 1) || true
	rm -rf kernel.bin kernel.iso obj