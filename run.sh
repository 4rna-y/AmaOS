set -euo pipefail

# イメージファイル名
IMG=fat.img

# 1) 空イメージの作成（64MiB）
dd if=/dev/zero of="$IMG" bs=1M count=64 status=progress

# 2) GPT ラベル＋FAT32 パーティション（ESP フラグ付き）を作成
sudo parted --script "$IMG" \
  mklabel gpt \
  mkpart primary fat32 1MiB 100% \
  set 1 esp on

# 3) ループデバイスにマッピング（パーティションも認識）
LOOP=$(sudo losetup --partscan --find --show "$IMG")

# 4) パーティションを FAT32 フォーマット
sudo mkfs.vfat -F32 "${LOOP}p1"

# 5) OVMF 変数領域ファイルをカレントにコピー（未コピーなら）
if [ ! -f OVMF_VARS.fd ]; then
  cp /usr/share/OVMF/OVMF_VARS.fd ./OVMF_VARS.fd
fi

# 6) ESP をマウントして EFI ローダーとカーネルを配置
MNT=$(mktemp -d)
sudo mount "${LOOP}p1" "$MNT"
sudo mkdir -p "$MNT/EFI/BOOT" "$MNT/amaos"
sudo cp ~/repos/edk2/Build/Loader/DEBUG_GCC5/X64/Loader.efi \
        "$MNT/EFI/BOOT/BOOTX64.EFI"
sudo cp ~/repos/osdev/Kernel/build/kernel.elf \
        "$MNT/amaos/kernel.elf"
sudo umount "$MNT"
rmdir "$MNT"

# 7) ループデバイス解除
sudo losetup -d "$LOOP"

# 8) QEMU 起動
qemu-system-x86_64 \
  -enable-kvm -cpu host -machine q35 -m 1024M \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
  -drive if=pflash,format=raw,file=./OVMF_VARS.fd \
  -drive id=disk,file=fat.img,format=raw,if=none \
  -device ahci,id=ahci \
  -device ide-hd,drive=disk,bus=ahci.0 \
  -device VGA,vgamem_mb=256 \
  -display gtk \
  -net none \
  -boot order=c -debugcon stdio \
  -no-reboot -no-shutdown -d int,cpu_reset