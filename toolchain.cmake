set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(TOOLS_DIR $ENV{HOME}/opt/cross)

set(CMAKE_C_COMPILER ${TOOLS_DIR}/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS_DIR}/bin/x86_64-elf-g++)
set(CMAKE_ASM_NASM_COMPILER ${TOOLS_DIR}/bin/nasm)
set(CMAKE_GRUB_COMMAND grub-mkrescue)

set(CMAKE_FIND_ROOT_PATH ${TOOLS_DIR})

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Required to make the previous line work for a target that requires a custom linker file
