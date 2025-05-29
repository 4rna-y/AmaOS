dd if=/dev/zero of=fat.img bs=1M count=64
mkfs.vfat fat.img
mkdir mnt
sudo mount -o loop fat.img mnt
sudo mkdir -p mnt/EFI/BOOT
sudo cp ~/repos/edk2/Build/Loader/DEBUG_GCC5/X64/Loader.efi mnt/EFI/BOOT/BOOTX64.EFI
sudo umount mnt
rmdir mnt

qemu-system-x86_64 \
    -machine q35 -m 1024M -bios /usr/share/OVMF/OVMF_CODE.fd \
    -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
    -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd \
    -drive file=fat.img,format=raw,if=virtio \
    -net none