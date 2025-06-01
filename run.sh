dd if=/dev/zero of=fat.img bs=1M count=64
mkfs.vfat fat.img
mkdir mnt
sudo mount -o loop fat.img mnt
sudo mkdir -p mnt/EFI/BOOT
sudo mkdir -p mnt/amaos
sudo cp ~/repos/edk2/Build/Loader/DEBUG_GCC5/X64/Loader.efi mnt/EFI/BOOT/BOOTX64.EFI
sudo cp ~/repos/osdev/Kernel/build/kernel.elf mnt/amaos/kernel.elf
sudo umount mnt
rmdir mnt

qemu-system-x86_64 \
    -enable-kvm -machine q35 -m 1024M -bios /usr/share/OVMF/OVMF_CODE.fd \
    -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
    -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd \
    -drive file=fat.img,format=raw,if=virtio \
    -device VGA,vgamem_mb=256 \
    -display gtk \
    -net none