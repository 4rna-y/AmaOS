[Defines]
    PLATFORM_NAME                  = LoaderPkg
    PLATFORM_GUID                  = 2dabc865-ff20-4486-b3a2-1d5c7ca07050
    PLATFORM_VERSION               = 0.1
    DSC_SPECIFICATION              = 0x00010005
    OUTPUT_DIRECTORY               = Build/Loader
    SUPPORTED_ARCHITECTURES        = X64
    BUILD_TARGETS                  = RELEASE

[LibraryClasses]
  UefiApplicationEntryPoint   | MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiLib                     | MdePkg/Library/UefiLib/UefiLib.inf

  BaseLib                     | MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib               | MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib                    | MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DevicePathLib               | MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  MemoryAllocationLib         | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PcdLib                      | MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PrintLib                    | MdePkg/Library/BasePrintLib/BasePrintLib.inf
  RegisterFilterLib           | MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  UefiBootServicesTableLib    | MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib | MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  StackCheckLib               | MdePkg/Library/StackCheckLib/StackCheckLib.inf
  StackCheckFailureHookLib    | MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf

[Components]
  LoaderPkg/Loader.inf