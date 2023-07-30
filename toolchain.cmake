set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(tools $ENV{HOME}/opt/cross)

set(CMAKE_C_COMPILER tools/bin/i686-elf-gcc)
set(CMAKE_CPP_COMPILER tools/bin/i686-elf-g++)

set(CMAKE_LD_FLAGS "")

set(CMAKE_FIND_ROOT_PATH tools)

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Required to make the previous line work for a target that requires a custom linker file
