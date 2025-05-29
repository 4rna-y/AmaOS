set -e

export EDK_PATH=~/repos/edk2
export PACKAGES_PATH=$(pwd):$EDK_PATH

cd $EDK_PATH
source edksetup.sh

make -C BaseTools

cd ~/repos/osdev

build -a X64 -t GCC5 -b DEBUG -p LoaderPkg/LoaderPkg.dsc