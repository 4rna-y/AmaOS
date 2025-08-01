set -e

export EDK_PATH=/home/rg/repos/edk2
export PACKAGES_PATH=$(pwd):$EDK_PATH

cd $EDK_PATH
source edksetup.sh

make -C BaseTools

cd /home/rg/repos/osdev

build -a X64 -t GCC5 -b RELEASE -p LoaderPkg/LoaderPkg.dsc